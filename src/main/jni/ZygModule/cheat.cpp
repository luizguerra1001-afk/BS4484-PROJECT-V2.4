#include <jni.h>
#include <dlfcn.h>
#include <thread>
#include <mutex>
#include <stdint.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

// SDK & Library Headers (ต้องมั่นใจว่ามีไฟล์เหล่านี้ในโปรเจกต์)
#include <dobby.h>
#include <esp.h>
#include <layout.h>
#include <imports.h>

// --- ประเภทข้อมูลพื้นฐาน ---
#define _QWORD uint64_t
#define _DWORD uint32_t
#define _WORD  uint16_t
#define _BYTE  uint8_t

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface) = nullptr;
static auto get_touchCount = (int (*)())nullptr;
static auto GetTouch = (UnityEngine_Touch_Fields(*)(int))nullptr;

// ตัวแปรสำหรับเก็บสถานะแสดงผล Toast
bool toast_target_lib_success = false;
bool toast_il2cpp_success = false;

// --- ImGui & EGL Hook Logic ---
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    if (!egl.setup) {
        eglQuerySurface(dpy, surface, EGL_WIDTH, &egl.width);
        eglQuerySurface(dpy, surface, EGL_HEIGHT, &egl.height);
        
        // โหลดฟังก์ชันการทัชหน้าจอจาก Unity Engine
        get_touchCount = (int (*)())Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"));
        GetTouch = (UnityEngine_Touch_Fields(*)(int))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1);
        
        // เริ่มต้นการใช้งาน ImGui Context
        ImGui::CreateContext();
        SetupImgui();
        egl.setup = true;

        // แสดงแจ้งเตือนโหลดสำเร็จหลังจาก ImGui พร้อมใช้งาน (กันแครช)
        if (toast_target_lib_success) {
            show_toast("Security Check", "SUCCESS", ImVec4(0.0f, 1.0f, 0.5f, 1.0f));
        } else {
            show_toast("Security Check", "FAIL / SKIPPED", ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
        }

        if (toast_il2cpp_success) {
            show_toast("Engine Status", "IL2CPP LOADED", ImVec4(0.0f, 1.0f, 0.5f, 1.0f));
        }
    }

    // สร้างอินพุตจำลองส่งเข้า ImGui สำหรับควบคุมเมนู
    ImGuiIO& io = ImGui::GetIO(); 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(egl.width, egl.height);
    ImGui::NewFrame();

    if (get_touchCount && get_touchCount() > 0) {
        UnityEngine_Touch_Fields touch = GetTouch(0);
        io.MousePos = ImVec2(touch.m_Position.fields.x, io.DisplaySize.y - touch.m_Position.fields.y);
        io.MouseDown[0] = (touch.m_Phase != TouchPhase::Ended && touch.m_Phase != TouchPhase::Canceled);
    } else {
        io.MouseDown[0] = false;
    }

    // ลำดับการวาด UI Overlay
    menu(); 
    DrawESP(egl.width, egl.height);
    update();

    ImGui::EndFrame(); 
    ImGui::Render(); 
    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
    return old_eglSwapBuffers(dpy, surface);
}

// --- ฟังก์ชันช่วยเหลือสำหรับระบบความปลอดภัย ---
void SafeHook(void* target, void* replace, void** backup, const char* debug_name) {
    if (target != nullptr) {
        DobbyHook(target, replace, backup);
    }
}

// เช็คความพร้อมของ Library ต่างๆ
void init_my_mod_menu() {
    void* libTarget = dlopen(OBFUSCATE("libanogs.so"), RTLD_NOLOAD | RTLD_NOW); 
    toast_target_lib_success = (libTarget != nullptr);

    // วนลูปเช็คจนกว่า Engine IL2CPP จะถูกโหลดขึ้น Memory ครบ 100%
    void* libIl2cpp = nullptr;
    while (!libIl2cpp) {
        libIl2cpp = dlopen(OBFUSCATE("libil2cpp.so"), RTLD_NOLOAD | RTLD_NOW);
        usleep(100000); // 0.1 วินาที
    }
    toast_il2cpp_success = true;
}

// --- ตัวเริ่มทำงานหลัก (Main Hook Thread) ---
void *FreeFire(const char *) {
    init_my_mod_menu();

    // ดักจับฟังก์ชันเรนเดอร์ภาพระบบ EGL
    void* libEGL = dlopen(OBFUSCATE("libEGL.so"), RTLD_NOW | RTLD_GLOBAL);
    if (!libEGL) { return nullptr; }
    DobbyHook((void *) dlsym(libEGL, OBFUSCATE("eglSwapBuffers")), (void *) hook_eglSwapBuffers, (void **) &old_eglSwapBuffers);
	
    Il2CppAttach(); 

    // หน่วงรอตัวแปรและฟังก์ชันพื้นฐานของ Engine โหลดเสร็จสิ้น
    void* openUrlPtr = nullptr;
    while (!openUrlPtr) {
        openUrlPtr = (void*)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Application"), OBFUSCATE("OpenURL"), 1);
        usleep(100000);
    }
    OpenURL = (void (*)(void*))openUrlPtr;

    // --- โครงสร้างการเขียน Hook แบบปลอดภัย (เปลี่ยนชื่อเป็น Generic สำหรับปรับแต่ง) ---
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsInSAPFlyJumping"), 0), (void*)hook_get_IsInSAPFlyJumping, (void**)&orig_get_IsInSAPFlyJumping, "get_IsInSAPFlyJumping");

	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("Send"), 4), (void*)_GameFacadeSend, (void**)&GameFacadeSend, "GameFacadeSend");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameConfig"), OBFUSCATE("get_ResetGuest"), 0), (void*)_ResetGuest, (void**)&ResetGuest, "ResetGuest");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("LateUpdate"), 0), (void*)_Gameupdate, (void**)&Gameupdate, "LateUpdate");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("LHNJLFOINDO"), 1), (void*)BLAGCMCGEJG1, (void**)&old_BLAGCMCGEJG1, "LHNJLFOINDO");//UPDATE
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetCurrentDashSpeed"), 0), (void*)hook_GetCurrentDashSpeed, (void**)&old_GetCurrentDashSpeed, "DashSpeed");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_MaxJumpHeight"), 0), (void*)my_get_MaxJumpHeight, (void**)&orig_get_MaxJumpHeight, "MaxJumpHeight");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_CustomGravity"), 0), (void*)my_get_CustomGravity, (void**)&orig_get_CustomGravity, "CustomGravity");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_RisingGravity"), 0), (void*)my_get_RisingGravity, (void**)&orig_get_RisingGravity, "RisingGravity");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetPhysXState"), 0), (void*)HSYN_GetPhysXState, (void**)&OSYN_GetPhysXState, "GetPhysXState");
    SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameSettingData"), OBFUSCATE("CachedSmoothHighFrame"), 0), (void*)_CachedSmoothHighFrame, (void**)&CachedSmoothHighFrame, "HighFPS");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsFoldWingGliding"), 0), (void*)_IsFoldWingGliding, (void**)&IsFoldWingGliding, "IsFoldWingGliding");
	SafeHook((void*) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("LMCBHLCPAJD"), OBFUSCATE("EAAIGDDHDDD"), 1), (void*)hook_SpeedBypass, (void **) &orig_SpeedBypass, "EAAIGDDHDDD");//UPDATE
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_InFallingState"), 0), (void*)_get_InFallingState, (void**)&get_InFallingState, "get_InFallingState");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsPoseFallingHigh"), 0), (void*)hook_IsPoseFallingHigh, (void**)&orig_IsPoseFallingHigh, "orig_IsPoseFallingHigh");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsIgnoreHighFalling"), 0), (void*)hook_IsIgnoreHighFalling, (void**)&orig_IsIgnoreHighFalling, "orig_IsIgnoreHighFalling");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.PhysicsModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("CharacterController"), OBFUSCATE("get_isGrounded"), 0), (void*)_isGroundedEngine, (void**)&isGroundedEngine, "isGroundedEngine");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIBigMapController"), OBFUSCATE("SendMapMarkChanged"), 5), (void*)_SendMapMarkChanged, (void**)&old_SendMapMarkChanged, "old_SendMapMarkChanged");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_ShowDamageNum"), 0), (void*)hook_get_ShowDamageNum, (void**)&orig_get_ShowDamageNum, "orig_get_ShowDamageNum");
  	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.HUD"), OBFUSCATE("UIHudNameController"), OBFUSCATE("ShowDamage"), 6), (void *)hook_ShowDamage, (void**) &orig_ShowDamage, "orig_ShowDamage");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetTargetDirection"), 0), (void*)GetTargetDirection_Hook, (void**)&old_GetTargetDirection, "old_GetTargetDirection");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetFallingDirection"), 0), (void*)GetFallingDirection_Hook, (void**)&old_GetFallingDirection, "old_GetFallingDirection");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetVelocity"), 0), (void*)GetVelocity_Hook, (void**)&old_GetVelocity, "old_GetVelocity");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("Update"), 0), (void*)_UpdateGranada, (void**)&UpdateGranada, "UpdateGranada");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("OnShowGrenadeLineChanged"), 0), (void*)_UpdateGranada, (void**)&UpdateGranada, "UpdateGranada");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_InSwapWeaponCD"), 0), (void*)_get_InSwapWeaponCD, (void**)&get_InSwapWeaponCD, "get_InSwapWeaponCD");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerAttributes"), OBFUSCATE("GetScatterRate"), 0), (void*)_ScatterRate, (void**)&ScatterRate, "ScatterRate");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("set_fieldOfView"), 1), (void*)hook_SetFOV, (void**)&orig_SetFOV, "orig_SetFOV");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIModelUser"), OBFUSCATE("get_UserLevel"), 0), (void*)_get_UserLevel, (void**)&get_UserLevel, "get_UserLevel");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_NickName"), 0), (void*)hook_get_NickName, (void**)&old_get_NickName, "old_get_NickName");
	SafeHook((void *) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameSettingData"), OBFUSCATE("IsHighFPS120Open"), 0), (void *)hook_IsHighFPS120Open, (void **) &orig_IsHighFPS120Open, "orig_IsHighFPS120Open");
    SafeHook((void *) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameSettingData"), OBFUSCATE("IsHighFPS144Open"), 0), (void *)hook_IsHighFPS144Open, (void **) &orig_IsHighFPS144Open, "orig_IsHighFPS144Open");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("NeedUseNewRoundTransition"), 0),(void*)hook_NeedTransition,(void**)&O_NeedTransition, "KuyKuy");
	SafeHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("ClientUsingVersion"), 0), (void*)hook_ClientVersion, (void**)&orig_ClientVersion, "ClientUsingVersion");
	    // โค้ดส่วนจัดการ Shader (ถ้าระบบเปิดอยู่)
    if (mlovinit()){
        setShader(OBFUSCATE("_AlphaMask"));
        LogShaders(); 
        Wallhack();
    }
    return nullptr;
}

// --- JNI Proxy System (เชื่อมต่อและส่งต่องานไปไลบรารีแท้) ---
JavaVM* jvm = nullptr; 
void* pLibRealUnity = nullptr;

typedef jint(JNICALL *CallJNI_OnLoad_t)(JavaVM *vm, void *reserved);
typedef void(JNICALL *CallJNI_OnUnload_t)(JavaVM *vm, void *reserved);

CallJNI_OnLoad_t RealJNIOnLoad = nullptr;
CallJNI_OnUnload_t RealJNIOnUnload = nullptr;

std::once_flag library_load_flag;

// ฟังก์ชันสำหรับแอบดักโหลดไลบรารีจริงอย่างเสถียร
bool LoadRealLibrary() {
    std::call_once(library_load_flag, []() {
        pLibRealUnity = dlopen(OBFUSCATE("libmainn.so"), RTLD_NOW | RTLD_GLOBAL);
        if (pLibRealUnity) {
            RealJNIOnLoad = reinterpret_cast<CallJNI_OnLoad_t>(dlsym(pLibRealUnity, OBFUSCATE("JNI_OnLoad")));
            RealJNIOnUnload = reinterpret_cast<CallJNI_OnUnload_t>(dlsym(pLibRealUnity, OBFUSCATE("JNI_OnUnload")));
        }
    });
    return (pLibRealUnity && RealJNIOnLoad);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm; // เซ็ตแชร์ค่าพอยน์เตอร์ไว้ใช้ในส่วนอื่น

    if (LoadRealLibrary()) {
        return RealJNIOnLoad(vm, reserved);
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    if (LoadRealLibrary() && RealJNIOnUnload) {
        RealJNIOnUnload(vm, reserved);
    }
}

// --- Constructor (ตัวจุดระบบเธรดเบื้องหลัง) ---
__attribute__((constructor))
void lib_main() {
    std::thread([]() {
        FreeFire(nullptr);
    }).detach();
}
