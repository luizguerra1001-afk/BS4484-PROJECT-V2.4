#pragma once
#include <cstdint>
#include <dlfcn.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

#define getRealOffset(offset) AgetAbsoluteAddress("libil2cpp.so",offset)

static uintptr_t libBase;
bool isGameLibLoaded = false;

// --- [ Helpers ] ---
monoString *il2cpp_string_new(const char *str) {
    static const auto __il2cpp_string_new = (monoString*(*)(const char*))dlsym(dlopen("libil2cpp.so", RTLD_NOLOAD), "il2cpp_string_new");
    return __il2cpp_string_new(str);
}

// --- [ Helpers เพิ่มเติม ] ---
void* il2cpp_object_new(void* klass) {
    // ใช้ dlopen แบบ RTLD_NOLOAD เพื่อดึง handle ของ lib ที่โหลดอยู่แล้วมาใช้
    static const auto __il2cpp_obj_new = (void*(*)(void*))dlsym(dlopen("libil2cpp.so", RTLD_NOLOAD), "il2cpp_object_new");
    if (!__il2cpp_obj_new) return nullptr;
    return __il2cpp_obj_new(klass);
}

// ประกาศโครงสร้างเพื่อให้คอมไพล์รู้จัก MonoString

long AfindLibrary(const char *library) {
    char filename[0xFF] = {0},
    buffer[1024] = {0};
    FILE *fp = NULL;
    long address = 0;
    sprintf(filename, OBFUSCATE("/proc/self/maps"));
    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (long) strtoul(buffer, NULL, 16);
            goto done;
        }
    }
    done:
    if (fp) {
        fclose(fp);
    }
    return address;
}

long AClibBase;
long AgetAbsoluteAddress(const char* libraryName, long relativeAddr) {
    if (AClibBase == 0) {
        AClibBase = AfindLibrary(libraryName);
        if (AClibBase == 0) {
            AClibBase = 0;
        }
    }
    return AClibBase + relativeAddr;
}
class Vvector3 {
public:
float X;
float Y;
float Z;
Vvector3() : X(0), Y(0), Z(0) {}
Vvector3(float x1, float y1, float z1) : X(x1), Y(y1), Z(z1) {}
Vvector3(const Vvector3 &v);
~Vvector3();
};

Vvector3::Vvector3(const Vvector3 &v) : X(v.X), Y(v.Y), Z(v.Z) {}
Vvector3::~Vvector3() {}

__attribute__((visibility("hidden")))
uintptr_t string2Offset(const char* s) {
    using conv_fn_t = unsigned long (*)(const char*, char**, int);
    conv_fn_t conv_fn = reinterpret_cast<conv_fn_t>(dlsym(RTLD_DEFAULT, OBFUSCATE("strtoul")));
    if constexpr (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return conv_fn(s, nullptr, 16);
    } else {
        using conv64_fn_t = unsigned long long (*)(const char*, char**, int);
        auto conv64_fn = reinterpret_cast<conv64_fn_t>(dlsym(RTLD_DEFAULT, OBFUSCATE("strtoull")));
        return conv64_fn(s, nullptr, 16);
    }
}

uintptr_t basePtr12,basePtr13,basePtr14,basePtr15;

__attribute__((visibility("hidden")))
auto LocalizarInderecoBase(const char* lib)
{
    uintptr_t InderecoBase = 0;
    char line[1024];
    char filename[0xFF] = {0};
    sprintf(filename,OBFUSCATE("/proc/self/maps"));
    FILE* fp = fopen(filename, OBFUSCATE("re"));
    if(fp) {
        while(fgets(line, sizeof line, fp)) {
            if(strstr(line, lib)) {
                InderecoBase = std::stoul(line, nullptr, 16);
                return InderecoBase;
            }
        }
    }
    return InderecoBase;
}


__attribute__((visibility("hidden")))
void* getAddressIL2CPP(uintptr_t relativeAddr2, bool recheck2 = false)
{
    while(basePtr13 == 0)
    {
        basePtr13 = LocalizarInderecoBase(OBFUSCATE("libil2cpp.so"));
        //LOGD(WRAPPER_MARCO("basePtr1: %p"), basePtr1);
    }
    if(recheck2)
        basePtr13 = LocalizarInderecoBase(OBFUSCATE("libil2cpp.so"));
    return (void*)(basePtr13 + relativeAddr2);
}
#define Class_Camera__get_main (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_main"))

void* get_main() {
    return reinterpret_cast<void* (__fastcall*)()>(Class_Camera__get_main)();
}

#define Class_Input__get_touchCount (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"))

#define Class_Input__GetTouch (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1)

#define Class_Input__get_mousePosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_mousePosition"))

#define Class_Screen__get_width (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_width"))
int get_width() {
    return reinterpret_cast<int(__fastcall*)()>(Class_Screen__get_width)();
}

#define Class_Screen__get_height (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_height"))
int get_height() {
    return reinterpret_cast<int(__fastcall*)()>(Class_Screen__get_height)();
}

#define Class_Screen__get_density (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_dpi"))

#define Camera_get_fieldOfView (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_fieldOfView"))
float get_fieldOfView() {
    return reinterpret_cast<float(__fastcall*)(void*)>(Camera_get_fieldOfView)(get_main());
}

#define Camera_set_fieldOfView (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("set_fieldOfView"), 1)
void* set_fieldOfView(float value) {
    return reinterpret_cast<void* (__fastcall*)(void*, float)>(Camera_set_fieldOfView)(get_main(), value);
}

#define ForWard (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_forward"), 0)
static Vector3 GetForward(void* player) {
    Vector3(*_GetForward)(void* players) = (Vector3(*)(void*))(ForWard);
    return _GetForward(player);
}

#define Class_Transform__GetPosition Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)
static Vector3 Transform_GetPosition(void* player) {
    Vector3 out = Vector3::zero();
    void (*_Transform_GetPosition)(void* transform, Vector3 * out) = (void (*)(void*, Vector3*))(Class_Transform__GetPosition);
    _Transform_GetPosition(player, &out);
    return out;
}

#define Class_Transform__SetPosition Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1)
static void Transform_INTERNAL_SetPosition(void* player, Vvector3 inn) {
    void (*Transform_INTERNAL_SetPosition)(void* transform, Vvector3 in) = (void (*)(void*, Vvector3))(Class_Transform__SetPosition);
    Transform_INTERNAL_SetPosition(player, inn);
}

#define Class_Transform__Position Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0)
Vector3 get_position(void* player) {
    Vector3(*_get_position)(void* players) = (Vector3(*)(void*))(Class_Transform__Position);
    return _get_position(player);
}

#define Class_Transform__Rotation Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_rotation"), 0)
static Quaternion GetRotation(void* player) {
Quaternion (*_GetRotation)(void* players) = (Quaternion(*)(void *))(Class_Transform__Rotation);
return _GetRotation(player);
}

#define Class_Camera__WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("WorldToScreenPoint"), 1)
static Vector3 WorldToScreenPoint(void* WorldCam, Vector3 WorldPos) {
    Vector3(*_WorldToScreenScene)(void* Camera, Vector3 position) = (Vector3(*)(void*, Vector3)) (Class_Camera__WorldToScreenPoint);
    return _WorldToScreenScene(WorldCam, WorldPos);
}
#define ListPlayer (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("DKCLHINMMFO"))
#define ListVehicle (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("LGNHODMMCMJ")) //OB55 UPDATE

#define EnemyUpdate (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("LateUpdate"), 0)

#define VehicleI_AmIn (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("VehicleIAmIn"), 0)
void* GetVehicleIAmIn(void* player) {
    if (!player) return nullptr;
    return ((void* (*)(void*))VehicleI_AmIn)(player);
}

#define MainCam (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("MainCameraTransform"))
static Vector3 CameraMain(void* player) {
    return get_position(*(void**)((uint64_t)player + MainCam));
}

#define Match (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0)
static void* Curent_Match() {
    void* (*_Curent_Match) (void* nuls) = (void* (*)(void*))(Match);
    return _Curent_Match(NULL);
}

#define Local (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIHudDetectorController"), OBFUSCATE("GetLocalPlayer"), 0)
static void* GetLocalPlayer(void* Game) {
    void* (*_GetLocalPlayer)(void* match) = (void* (*)(void*))(Local);
    return _GetLocalPlayer(Game);
}

#define Visible (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsVisible"), 0)
static bool get_isVisible(void* player) {
    bool (*_get_isVisible)(void* players) = (bool (*)(void*))(Visible);
    return _get_isVisible(player);
}

#define Team (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsLocalTeammate"), 1)
static bool get_isLocalTeam(void* player) {
    using fnGetIsLocalTeam = bool(*)(void*, bool);
    auto _get_isLocalTeam = reinterpret_cast<fnGetIsLocalTeam>(Team);
    return _get_isLocalTeam(player, false);
}

#define Die (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0)
static bool get_IsDieing(void* player) {
    bool (*_get_die)(void* players) = (bool (*)(void*))(Die);
    return _get_die(player);
}

#define CurHP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_CurHP"), 0)
static int GetHp(void* player) {
    int (*_GetHp)(void* players) = (int(*)(void*))(CurHP);
    return _GetHp(player);
}

#define MaxHP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_MaxHP"), 0)
static int get_MaxHP(void* enemy) {
    int (*_get_MaxHP)(void* player) = (int(*)(void*))(MaxHP);
    return _get_MaxHP(enemy);
}

#define Name (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_NickName"), 0)
static monoString* get_NickName(void* player) {
    monoString* (*_get_NickName)(void* players) = (monoString * (*)(void*))(Name);
    return _get_NickName(player);
}

#define Aim (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("SetAimRotation"), 2) //0
static void set_aim(void* player, Quaternion look) {
    using fnSetAim = void(*)(void*, Quaternion, bool);
    auto _set_aim = reinterpret_cast<fnSetAim>(Aim);

    _set_aim(player, look, false);
}

#define Scope (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsSighting"),0 )
static bool get_IsSighting(void* player) {
    bool (*_get_IsSighting)(void* players) = (bool (*)(void*))(Scope);
    return _get_IsSighting(player);
}

#define Fire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsFiring"), 0)
static bool get_IsFiring(void* player) {
    bool (*_get_IsFiring)(void* players) = (bool (*)(void*))(Fire);
    return _get_IsFiring(player);
}

#define Head (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0)
static void* GetHeadPositions(void* player) {
    void* (*_GetHeadPositions)(void* players) = (void* (*)(void*))(Head);
    return _GetHeadPositions(player);
}

#define CharGet (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System"), OBFUSCATE("String"), OBFUSCATE("get_Chars"), 1) //0
char get_Chars(monoString* str, int index) {
    char (*_get_Chars)(monoString * str, int index) = (char (*)(monoString*, int))(CharGet);
    return _get_Chars(str, index);
}

#define HeadColider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_HeadCollider"))
static void* Player_GetHeadCollider(void* player) {
    void* (*_Player_GetHeadCollider)(void* players) = (void* (*)(void*))(HeadColider);
    return _Player_GetHeadCollider(player);
}

#define offset_LocalPlayer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentLocalPlayer"), 0)
static void *Current_Local_Player() {
    void *(*_Local_Player)(void *players) = (void *(*)(void *))(offset_LocalPlayer);
    return _Local_Player(NULL);
}

#define Hip (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHipTF"), 0)
static void* GetHipPositions(void* player) {
void* (*_GetHipPositions)(void* players) = (void*(*)(void*))(Hip);
return _GetHipPositions(player);
}

#define Class_Compent__Transform Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
static void *Component_GetTransform(void *player) {
void *(*_Component_GetTransform)(void *component) = (void *(*)(void *))(Class_Compent__Transform);
return _Component_GetTransform(player);
}

static void *Camera_main() {
void *(*_Camera_main)(void *nuls) = (void *(*)(void *))(Class_Camera__get_main);
return _Camera_main(nullptr);
}

Vector3 getPosition(void *transform) {
return get_position(Component_GetTransform(transform));
}
static Vector3 GetHeadPosition(void* player) {
return get_position(GetHeadPositions(player));
}
static Vector3 GetHipPosition(void* player) {
return get_position(GetHipPositions(player));
}

float get_density() {
return reinterpret_cast<float(__fastcall *)()>(Class_Screen__get_density)();
}


#define Imo (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetActiveWeapon"))
static void* get_imo(void* player) {
    void* (*_GetImo)(void* players) = (void* (*)(void*))Imo;
    return _GetImo(player);
}


#define Esp2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("LevelMiniSentry"), OBFUSCATE("GENFKNDFIJH"), 2) //OB55 UPDATE
static void set_esp2(void* imo, Vector3 x, Vector3 y) {
    void (*_SetEsp2)(void* imo, Vector3 X, Vector3 Y) = (void (*)(void*, Vector3, Vector3))Esp2;
    _SetEsp2(imo, x, y);
}

#define TranGetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)
static Vector3 Transform_INTERNAL_GetPosition(void *player) {
    Vector3 out = Vector3::zero();
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))(TranGetPosition);
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}

#define offset_GetWeaponOnHand (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetWeaponOnHand"), 0)
static void *GetWeaponOnHand(void *local) {
    void *(*_GetWeaponOnHand)(void *local) = (void *(*)(void *))(offset_GetWeaponOnHand);
    return _GetWeaponOnHand(local);
}


#define GameObject Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_gameObject"), 0)
void *get_gameObject(void *player)
{
    return ((void *(*)(void *))(GameObject))(player);
}

#define HeadTF (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0)
void *GetHeadTF(void *player)
{
    return ((void* (*)(void*))(HeadTF))(player);
}

#define Raycast (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("IPBGFJNGLOH"), OBFUSCATE("FFLLOGJDGOJ"), 4) //OB55 UPDATE
static bool Physics_Raycast(Vector3 start, Vector3 end, unsigned int flag, void* hitInfo) {
    // กำหนดรูปแบบฟังก์ชันให้ตรงกับที่ Dump บอก
    auto _Physics_Raycast = (bool(*)(Vector3, Vector3, unsigned int, void*))(Raycast);
    return _Physics_Raycast(start, end, flag, hitInfo);
}

#define offset_StopFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NFFJEBKJCAK"), 0)//OB55 UPDATE
#define offset_StartFiringer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("ALJMFCJMDCF"), 0)//OB55 UPDATE
#define offset_GetDelay (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("GGKJGKMMPEH"), 0)//OB55 UPDATE
#define m_DamageRange (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("OEOBPHDFJMO"), 0)//OB55 UPDATE

static float get_Range(void* pthis)
{
    return ((float (*)(void*))(m_DamageRange))(pthis);
}
static float get_Delay(void *pthis)
{
    return ((float (*)(void *))(offset_GetDelay))(pthis);
}
static void *StopFiring(void *weapon) {
    void *(*_StopFiring)(void *Weapon) = (void *(*)(void *))(offset_StopFiring);
    return _StopFiring(weapon);
}
static void *StartFiring(void *weapon) {
    void *(*_StartFiring)(void *Weapon) = (void *(*)(void *))(offset_StartFiringer);
    return _StartFiring(weapon);
}
#define m_isClientBot (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsClientBot"))

#define Creep (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsCreep"), 0)
static bool get_IsCreep(void* player) {
    bool (*_get_IsCreep)(void* players) = (bool (*)(void*))(Creep);
    return _get_IsCreep(player);
}

#define Walk (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsWalking"), 0)
static bool get_IsWalk(void* player) {
    bool (*_get_IsWalk)(void* players) = (bool (*)(void*))(Creep);
    return _get_IsWalk(player);
}

#define offset_SwitchPhysXState (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("SwitchPhysXState"), 2)

#define offset_set_positionCount (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("set_positionCount"), 1)
#define offset_SetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("SetPosition"), 2)
#define offset_DrawLine2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("DrawLine2"), 3)
#define offset_GrenadeLine (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_GrenadeLine"))
#define offset_ShowGrenadeLine (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_ShowGrenadeLine"))
#define offset_ShowThrowSkillLine (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_ShowThrowSkillLine"))
#define offset_set_startColor (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("set_startColor"), 1)
#define offset_set_endColor (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("set_endColor"), 1)

struct UnityColor {
    float r, g, b, a;
};

static void LineRenderer_SetPosition(void *Render, int value, Vector3 Location){
    void (*_LineRenderer_SetPosition)(void *Render, int value, Vector3 Location) = (void (*)(void*, int, Vector3))(offset_SetPosition);
    return _LineRenderer_SetPosition(Render, value, Location);
}

static void LineRenderer_Set_PositionCount(void *Render, int value){
    void (*_LineRenderer_Set_PositionCount)(void *Render, int value) = (void (*)(void*, int))(offset_set_positionCount);
    return _LineRenderer_Set_PositionCount(Render, value);
}

static void LineRenderer_SetColor(void *Render, UnityColor color){
    void (*_set_startColor)(void *Render, UnityColor color) = (void (*)(void*, UnityColor))(offset_set_startColor);
    void (*_set_endColor)(void *Render, UnityColor color) = (void (*)(void*, UnityColor))(offset_set_endColor);
    _set_startColor(Render, color);
    _set_endColor(Render, color);
}

static void GrenadeLine_DrawLine(void *instance, Vector3 start, Vector3 end, Vector3 position) {
    void (*_GrenadeLine_DrawLine)(void *clz, Vector3 throwPos, Vector3 throwVel, Vector3 gravity) = (void (*)(void*, Vector3, Vector3,Vector3)) (offset_DrawLine2);
    return _GrenadeLine_DrawLine(instance, start, end, position);
}

void *LineGrenade = nullptr;
void *RenderLine = nullptr;

#define offset_ShowGrenadeLine (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_ShowGrenadeLine")) 
#define offset_GrenadeLine (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_GrenadeLine"))

void (*UpdateGranada)(void *_this);
void _UpdateGranada(void *_this) {
    if (_this != NULL) {
        LineGrenade = _this;
        *(bool *)((long)_this + offset_ShowGrenadeLine) = true;
        RenderLine = *(void **)((long)_this + offset_GrenadeLine);
    }
    UpdateGranada(_this);
}
#define offset_StartOnGrapplingHook (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("StartOnGrapplingHook"), 0)
static void StartOnGrapplingHook(void *Player) {
    if (!Player) return;
    void (*_StartOnGrapplingHook)(void *) = (void (*)(void *))(offset_StartOnGrapplingHook);
    _StartOnGrapplingHook(Player);
}

#define offset_StartSAPGlid (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("StartSAPGlid"), 0)
#define offset_StartSAPJumpBeforeGlid (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("StartSAPJumpBeforeGlid"), 0)

static void StartSAPGlid(void *Player) {
    if (!Player) return;
    void (*_StartSAPGlid)(void *) = (void (*)(void *))(offset_StartSAPGlid);
    _StartSAPGlid(Player);
}

static void StartSAPJumpBeforeGlid(void *Player) {
    if (!Player) return;
    void (*_StartSAPJumpBeforeGlid)(void *) = (void (*)(void *))(offset_StartSAPJumpBeforeGlid);
    _StartSAPJumpBeforeGlid(Player);
}

#define offset_StartFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartFiring"), 1)
#define offset_StopFire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StopFire"), 1)
#define offset_StartWholeBodyFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartWholeBodyFiring"), 1)

static void StartFiring(void *Player, void *WeaponOnHand) {
    void (*_StartFiring)(void *, void *) = (void (*)(void *, void *))(offset_StartFiring);
    return _StartFiring(Player, WeaponOnHand);
}

static void StopFire1(void* Player,void* WeaponOnHand){
    void(*_StopFire1)(void*,void*) = (void(*)(void*,void*))(offset_StopFire);
    return _StopFire1(Player,WeaponOnHand);
}

void StartWholeBodyFiring(void* Player,void* WeaponOnHand){
    void(*_StartWholeBodyFiring)(void*,void*) = (void(*)(void*,void*))(offset_StartWholeBodyFiring);
    return _StartWholeBodyFiring(Player,WeaponOnHand);
}

#define _playerAttributes (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IODLOCEJIOK"))//OB55 UPDATE
#define offset_NoReload (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerAttributes"), OBFUSCATE("ShootNoReload"))

static void set_skybox_Null(void* material) {
    void (*fn)(void*) = (void (*)(void*))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("RenderSettings"), OBFUSCATE("set_skybox"), 1);
    fn(material);
}
static void* get_skybox_Original() {
    void* (*fn)() = (void* (*)())Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("RenderSettings"), OBFUSCATE("get_skybox"), 0);
    return fn();
}
static void set_ambientMode_Black(int mode) {
    void (*fn)(int) = (void (*)(int))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("RenderSettings"), OBFUSCATE("set_ambientMode"), 1);
    fn(mode);
}

#define offset_BaseDamage (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("HOBPLJGNDEA")) //OB55 UPDATE

#define m_Timer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentGameSimulationTimer"), 0)
static void* GetSimulationTimer()
{
    void* (*_GetSimulationTimer) () = reinterpret_cast<void* (*)()>(m_Timer);
    return _GetSimulationTimer();
}

#define m_GetTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("get_FixedDeltaTime"), 0)
static float GetTimer(void* timeServiceInstance)
{
    using fnGetFixedDelta = float(*)(void*);
    auto _GetTimer = reinterpret_cast<fnGetFixedDelta>(m_GetTimer);
    return _GetTimer(timeServiceInstance);
}

#define m_SetTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("UseFixedDeltaTime"), 1)
static void SetTimer(void* timeServiceInstance, float fixedDelta)
{
    using fnSetFixed = void(*)(void*, float);
    auto _set_fixed = reinterpret_cast<fnSetFixed>(m_SetTimer);
    _set_fixed(timeServiceInstance, fixedDelta);
}

#define Esp1 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("GENFKNDFIJH"), 4)//OB55 UPDATE
static void set_esp1(void* imo, Vector3 x, Vector3 y, void* obj) {
    void (*_SetEsp1)(void*, Vector3, Vector3, void*) = (void (*)(void*, Vector3, Vector3, void*))Esp1;
    _SetEsp1(imo, x, y, obj);
}

#define PlayGunTrace (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("UGCLevelMiniSentry"), OBFUSCATE("GENFKNDFIJH"), 2)//OB55 UPDATE
static void set_esp3(void* imo, Vector3 x, Vector3 y) {
    void (*_set_esp3)(void *imo, Vector3 X, Vector3 Y) = (void (*)(void *, Vector3, Vector3))(PlayGunTrace);
    _set_esp3(imo, x, y);
}

#define offset_DelayEquipBoard (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("DelayEquipBoard"), 0)
static void DelayEquipBoardUpdate(void* player) {
    void (*fn)(void*) = (void (*)(void*))offset_DelayEquipBoard;
    fn(player);
}

#define Scope_Explode (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Vehicle"), OBFUSCATE("Explode"), 1)
#define Scope_Dead    (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Vehicle"), OBFUSCATE("Dead"), 1)

typedef void (*Explode_t)(void* instance, float damage);
typedef void (*Dead_t)(void* instance, float value);

inline void TriggerExplosion(void* vehicleInstance) {
    if (vehicleInstance == nullptr) return;
    uintptr_t rva = Scope_Explode;
    if (rva != 0) {
        ((Explode_t)rva)(vehicleInstance, 0.0f);
    }
}

inline void TriggerVehicleDead(void* vehicleInstance) {
    if (vehicleInstance == nullptr) return;
    uintptr_t rva = Scope_Dead;
    if (rva != 0) {
        ((Dead_t)rva)(vehicleInstance, 0.0f); 
    }
}

#define Scope_UnLockSpeed (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Vehicle"), OBFUSCATE("UnLockSpeed"), 0)
typedef void (*UnLockSpeed_t)(void* instance);

inline void TriggerUnLockSpeed(void* vehicleInstance) {
    if (vehicleInstance == nullptr) return;
    uintptr_t rva = Scope_UnLockSpeed;
    if (rva != 0) {
        ((UnLockSpeed_t)rva)(vehicleInstance);
    }
}
#define Scope_LockSpeed (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Vehicle"), OBFUSCATE("LockSpeed"), 0)

typedef void (*LockSpeed_t)(void* instance);

inline void TriggerLockSpeed(void* vehicleInstance) {
    if (vehicleInstance == nullptr) return;
    uintptr_t rva = Scope_LockSpeed;
    if (rva != 0) {
        ((LockSpeed_t)rva)(vehicleInstance);
    }
}

#define CenterWS (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetAttackableCenterWS"))

static Vector3 GetAttackableCenterWS(void* player) {
    Vector3 (*_GetAttackableCenterWS)(void*) = (Vector3 (*)(void*))CenterWS;
    return _GetAttackableCenterWS(player);
}
#define m_currentUi Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentInGameUIScene"), 0)
static void* CurrentInGameUIScene() {
    using fnCurrentUIScene = void* (*)();
    auto _CurrentUIScene = reinterpret_cast<fnCurrentUIScene>(m_currentUi);
    return _CurrentUIScene();
}
#define offset_ShowTeammateTips (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowCenterUpTeammateTips"), 2)
static void ShowCenterUpTeammateTips(monoString *message, float duration = 3.0f) {
    if (offset_ShowTeammateTips == 0) return;    
    void (*_Show)(void *, monoString *, float) = (void (*)(void *, monoString *, float))(offset_ShowTeammateTips);    
    void *ui = CurrentInGameUIScene(); 
    if (ui != nullptr) {
        _Show(ui, message, duration);
    }
}

#define SendFootBallChange1 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("SendChangeToGBFootBall"), 0)
static void Call_SendFootBallChange1(void* instance) {
    if (instance == nullptr) return;
    ((void (*)(void*))SendFootBallChange1)(instance);
}
#define SendCanFootBallChange1 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("SendCancelGBFootBall"), 0)
static void Call_SendCanFootBallChange1(void* instance) {
    if (instance == nullptr) return;
    ((void (*)(void*))SendCanFootBallChange1)(instance);
}


#define offset_swapweaopon (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("SwapWeapon"), 3)
void SwapWeapon(void *Pthis,int32_t FANMJANBFIL,bool GDKLMFLNNGM)
{
    return ((void (*)(void *,int,bool,void*))offset_swapweaopon)(Pthis,FANMJANBFIL,GDKLMFLNNGM,nullptr);
}
#define m_addTeamHud Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowAssistantText"), 2)
static void ShowAssistantText(void* uiInstance, monoString* playerName, monoString* line) {
    using fnShowAssistantText = void(*)(void*, monoString*, monoString*);
    auto _ShowAssistantText = reinterpret_cast<fnShowAssistantText>(m_addTeamHud);
    if (uiInstance != nullptr) {
        _ShowAssistantText(uiInstance, playerName, line);
    }
}
static monoString* U3DStrFormat(float distance, int curHp, int maxHp, const char* weaponName) {
    char buffer[256] = {0};
    sprintf(buffer, OBFUSCATE("[BS4484][HP: %d] distance: %.2fm"), curHp, distance);
    return il2cpp_string_new(buffer);
}
std::string GetWeaponNameByID(int id) {
    switch (id) {
        case 0: return "VECTOR-DOUBLE";
        case 1: return "FIST";
        case 2: return "M4A1";
        case 3: return "USP";
        case 4: return "AWM";
        case 5: return "M1014";
        case 6: return "AK-47";
        case 7: return "UMP";
        case 8: return "MP5";
        case 9: return "DESERT ENGLE";
        case 10: return "G18";
        case 11: return "M14";
        case 12: return "SCAR";
        case 13: return "VSS";
        case 14: return "GROZA";
        case 15: return "MP40";
        case 16: return "PAN";
        case 17: return "PARANG";
        case 18: return "SKS";
        case 19: return "M249";
        case 20: return "M1873";
        case 25: return "M500";
        case 26: return "SVD";
        case 27: return "BAT";
        case 28: return "XM8";
        case 29: return "SPAS12";
        case 30: return "M60";
        case 32: return "P90";
        case 33: return "AN94";
        case 34: return "KATANA";
        case 35: return "CG15";
        case 39: return "PLASMA";
        case 41: return "M1887";
        case 46: return "AUG";
        case 45: return "M82B";
        case 47: return "PARAFAL";
        case 48: return "WOODPECKER";
        case 49: return "VECTOR";
        case 50: return "MAG-7";
        case 51: return "SCYTHE";
        case 54: return "KORD";
        case 55: return "M1917";
        case 56: return "USP-2";
        case 57: return "KINGFISHER";
        case 58: return "MNI UZI";
        case 60: return "MP5-LV1";
        case 61: return "M60-LV1";
        case 63: return "M14-LV1";
        case 65: return "AWM-Y";
        case 70: return "GROZA-X";
        case 71: return "M249-X";
        case 72: return "SVD-Y";
        case 74: return "G36";
        case 75: return "M24";
        case 78: return "HEALING SNIPER";
        case 80: return "M4A1-LV1";
        case 81: return "M4A1-LV2";
        case 82: return "M4A1-LV3";
        case 86: return "CHARGE BUSTER";
        case 88: return "MAC10";
        case 89: return "AC80";
        case 93: return "HEAL PISTOL";
        case 99: return "SHIELD GUN";
        case 100: return "FLAMETHROWER";
        case 119: return "M1887-X";
        case 120: return "MP5-LV2";
        case 121: return "MP5-LV3";
        case 122: return "M60-LV2";
        case 123: return "M60-LV3";
        case 126: return "M14-LV2";
        case 127: return "M14-LV3";
        case 129: return "KAR98K";
        case 131: return "FAMAS";
        case 149: return "FF KNIFE";
        case 150: return "BIZON";
        case 178: return "SCAR-LV1";
        case 179: return "SCAR-LV2";
        case 180: return "SCAR-LV3";
        case 181: return "TROGON";
        case 184: return "M1014-LV1";
        case 185: return "M1014-LV2";
        case 186: return "M1014-LV3";
        case 193: return "AUG-LV1";
        case 194: return "AUG-LV2";
        case 195: return "AUG-LV3";
        case 197: return "VSK94";
        case 228: return "MAC10-LV1";
        case 229: return "MAC10-LV2";
        case 230: return "MAC10-LV3";
        case 601: return "GRENADE";
        case 608: return "FLASH FREEZE";
        case 617: return "GLOO MELTER";
        case 633: return "SVD-Y";
        case 1204: return "GLOO WALL";
        case 21001: return "HEAL PISTOL-Y";
        case 21002: return "M590";
        case 21006: return "WINCHEASTER";
        case 21007: return "WINCHEASTER-LV1";
        case 21008: return "WINCHEASTER-LV2";
        case 21009: return "WINCHEASTER-LV3";
        case 21022: return "THOMPSON";
        case 21020: return "THOMPSON-X";
        case 50015: return "HEAL UAV";
        default: return "Weapon Code : " + std::to_string(id);
    }
}

#include <cstdint>

#define BNJIPPIIMDF1616 Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("HIEMGFMAPJO"), OBFUSCATE(".ctor"), 0) //UPDATE OB55
#define offset_PNGAJBCPDNJ (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PhyXShield"), OBFUSCATE("GLEMOCGDGHM"), 0) //UPDATE OB55
#define offset_KFMGKCJMCAM (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("MHGFKKALMDK")) //UPDATE OB55
#define offset_GEGFCFDGGGP (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("FDMIEDDNCEC")) //UPDATE OB55
#define offset_SetStartDamage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("LHNJLFOINDO"), 1) //UPDATE OB55
#define offset_GetWeaponID (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("IEFJGCGOMID"), OBFUSCATE("MJCENIGKEPH"), 0) //UPDATE OB55
#define offset_GKHECDLGAJA (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("JGIEOGCGFEP"), 1) //UPDATE OB55
#define offset_LCLHHHKFCFP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NPLMKLBKJPE"), 4) //UPDATE OB55
#define Classdamageinfo Il2CppGetClassType(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HNLIMBLIANL")) //UPDATE OB55
#define offset_TakeDamage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("TakeDamage"), 9) //UPDATE OB55
#define offset_IJBAPAJAINA (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NCHOCBKLNEJ"), 2) //UPDATE OB55

static int GetWeapon(void* enemy) {
   	 int (*GetWeapon)(void *player) = (int(*)(void *))(offset_GetWeaponID);
  	 return GetWeapon(enemy);
}
	
#if defined(__aarch64__)
#define GetUniqueID(WeaponHand) *(uint32_t*)((uint64_t)WeaponHand + 0x10)
#else
#define GetUniqueID(WeaponHand) *(uint32_t*)((uint64_t)WeaponHand + 0x8)
#endif

struct PlayerID {
    uint32_t m_Value;
    uint32_t m_ID;
    uint8_t m_TeamID;
    uint8_t m_ShortID;
    uint64_t m_IDMask;
};

struct DamageInfo2_o {
#if defined(__arm__)
    void *klass;                 // 0x00
    void *monitor;               // 0x04
    int32_t BaseDamage;          // 0x08
    int32_t HitColliderType;     // 0x0C
    monoString* HitColliderName; // 0x10
    bool isBackArea;             // 0x14
    char pad_0x15[3];            // 0x15
    uint32_t PlayerID;           // 0x18
    char pad_0x1C[24];           // 0x1C
    void* WeaponHand;            // 0x34
    int32_t weaponID;            // 0x38
    Vector3 firePos;             // 0x3C
    Vector3 hitPos;              // 0x48
    Vector3 hitN;                // 0x54
    int16_t SpecialHitType;      // 0x60 (เปลี่ยนจาก pad_0x60 เป็นตัวแปรจริง ขนาด 2 ไบต์)
    bool ForceNoHeadShot;        // 0x62 (ขนาด 1 ไบต์)
    char pad_0x63;               // 0x63 (ถม 1 ไบต์เพื่อให้ตัวแปรถัดไปเริ่มที่ตำแหน่งหาร 4 ลงตัว)
    int32_t ExtraInfo;           // 0x64 (ขนาด 4 ไบต์)
    void* SpecialHitDic;         // 0x68 (Pointer ขนาด 4 ไบต์)
#else
    void *klass;                 // 0x00
    void *monitor;               // 0x08
    int32_t BaseDamage;          // 0x10
    int32_t HitColliderType;     // 0x14
    monoString* HitColliderName; // 0x18
    bool isBackArea;             // 0x20
    char pad_0x21[7];            // 0x21
    uint32_t PlayerID;           // 0x28
    char pad_0x2C[20];           // 0x2C
    void* WeaponHand;            // 0x40
    int32_t weaponID;            // 0x48
    Vector3 firePos;             // 0x4C
    Vector3 hitPos;              // 0x58
    Vector3 hitN;                // 0x64
    int16_t SpecialHitType;      // 0x70 (เปลี่ยนจาก pad_0x70 เป็นตัวแปรจริง ขนาด 2 ไบต์)
    bool ForceNoHeadShot;        // 0x72 (ขนาด 1 ไบต์)
    char pad_0x73;               // 0x73 (ถม 1 ไบต์เพื่อขยับไปตำแหน่ง 0x74)
    int32_t ExtraInfo;           // 0x74 (ขนาด 4 ไบต์)
    void* SpecialHitDic;         // 0x78 (Pointer ขนาด 8 ไบต์)
#endif
};



#if defined(__arm__)
struct GMPGMPFNMFP {
    void *klass;                // IL2CPP Class Header
    void *monitor;              // IL2CPP Monitor/Lock
    bool  m_IsInPool;           // ObjectPool State (มาจากคลาสแม่)
    char  pad_32bit_1[3];       // ปรับ Alignment 32-bit
    void *HLIJMDODPIM;          // GameObject (HitObject)
    void *OCEBCHENIOK;          // Collider (HitCollider)
    Vector3 MBGBCLNJOMK;        // Vector3 (HitLocation)
    Vector3 DGFLGBEOGPG;        // Vector3 (HitNormal)
    Vector3 IKDEGKIICJP;        // Vector3 (RayDir)
    Vector3 LMAEGPEAECO;        // Vector3 (StartPosition)
    int32_t BGKGJKDILEA;        // int (Damage) ** ดาเมจอยู่นี่ **
    float   PGCPFOAJHBM;        // float (Distance)
    int32_t CMPLENABKPC;        // int (ActorLayer)
    int32_t   FLCLOHCBJEI;        // Enum (EHitGroup - ส่วนหัว/ส่วนตัว)
    void *CICAFKKFBDL;          // PhysicMaterial (HitPhysicMaterial)
    bool DEDOKPCAHAC;           // bool (IgnoreHappens)
    bool GGJOADOBLID;           // bool (ViewBlocked)
    char pad_32bit_2[2];        // ปรับ Alignment 32-bit
    Vector3 KPEICEMCHIF;        // Vector3 (OrigStartPosition)
    int16_t LNOIFBAFGOK;        // short (SpecialHitType)
    char pad_32bit_3[2];        // ปรับ Alignment 32-bit
    void* GBOLEJPOAEP;          // Dictionary (SpecialHitDic)
    void* EMGLFDCDEBG;          // string (UGCLogicEntityID)
};
#else
struct GMPGMPFNMFP {
    void *klass;                // IL2CPP Class Header
    void *monitor;              // IL2CPP Monitor/Lock
    bool  m_IsInPool;           // ObjectPool State (มาจากคลาสแม่)
    char  pad_01[7];            // ปรับ Alignment 64-bit
    void *HLIJMDODPIM;          // GameObject (HitObject)
    void *OCEBCHENIOK;          // Collider (HitCollider)
    Vector3 MBGBCLNJOMK;        // Vector3 (HitLocation)
    Vector3 DGFLGBEOGPG;        // Vector3 (HitNormal)
    Vector3 IKDEGKIICJP;        // Vector3 (RayDir)
    Vector3 LMAEGPEAECO;        // Vector3 (StartPosition)
    int32_t BGKGJKDILEA;        // int (Damage)
    float   PGCPFOAJHBM;        // float (Distance)
    int32_t CMPLENABKPC;        // int (ActorLayer)
    int32_t FLCLOHCBJEI;        // Enum (EHitGroup) **แก้จาก void* เป็น int32_t**
    void *CICAFKKFBDL;          // PhysicMaterial (HitPhysicMaterial)
    bool DEDOKPCAHAC;           // bool (IgnoreHappens)
    bool GGJOADOBLID;           // bool (ViewBlocked)
    char pad_02[2];             // ปรับ Alignment 64-bit
    Vector3 KPEICEMCHIF;        // Vector3 (OrigStartPosition)
    int16_t LNOIFBAFGOK;        // short (SpecialHitType)
    char pad_03[6];             // ปรับ Alignment 64-bit
    void* GBOLEJPOAEP;          // Dictionary (SpecialHitDic)
    void* EMGLFDCDEBG;          // string (UGCLogicEntityID)
};
#endif

struct GCommon_TimeService_o {
    void *klass;
    void *monitor;
    float m_GameTime;
    float m_LastGameTime;
    float m_DeltaTime;
    uint32_t m_DeltaTickCount;
    uint32_t m_TickCount;
    uint32_t m_RealDoLogicTickCount;
    bool m_UsingFixedDeltaTime;
    float m_FixedDeltaTime;
};

namespace Save {
    void* DamageInfo;
}

static float lastDamageTime = 0;
float TakeRate = 0.5f;

void *GKHECDLGAJA(void *pthis, void* a1) {
    return ((void* (*)(void *, void *))(offset_GKHECDLGAJA))(pthis, a1);
}

monoList<float *> *LCLHHHKFCFP(void *Weapon, void *CAGCICACKCF, void *HFBDJJDICLN, bool LDGHPOPPPNL, DamageInfo2_o *DamageInfo) {
    return ((monoList<float *> * (*)(void*, void*, void*, bool, DamageInfo2_o*))(offset_LCLHHHKFCFP))(Weapon, CAGCICACKCF, HFBDJJDICLN, LDGHPOPPPNL, DamageInfo);
}

static int32_t TakeDamage(void *_this, int32_t baseDamage, PlayerID damager, DamageInfo2_o* damageInfo, int32_t weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float *>* checkParams, void* damagerWeaponDynamicInfo, uint32_t vehicleDataID) {
    return ((int32_t (*)(void*, int32_t, PlayerID, DamageInfo2_o*, int32_t, Vector3, Vector3, monoList<float *> *, void*, uint32_t))(offset_TakeDamage))(_this, baseDamage, damager, damageInfo, weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, vehicleDataID);
}
