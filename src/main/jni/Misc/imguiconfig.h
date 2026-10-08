#include "imgui_impl_opengl3.h"
#include "imgui_impl_android.h"
#include <imgui.h>
#include <memory.h>
#include "SYNZ.h"
#include "SYNZ1.h"


bool Speed = true;
float (*EXEMPLO) (void* as);
float _EXEMPLO(void* as){
if (Speed){
return 1.9f;
}
return EXEMPLO(as);
}



struct {
    bool setup;
    int width;
    int height;
    int screenWidth;
    int screenHeight;
} egl;

static float scaleX = 1.0f;
static float scaleY = 1.0f;

void (*old_input)(void *event, void *exAb, void *exAc);
void hook_input(void *event, void *exAb, void *exAc) {
    if (egl.width > 0 && egl.height > 0) {
        scaleX = (float)egl.screenWidth / (float)egl.width;
        scaleY = (float)egl.screenHeight / (float)egl.height;
    }
    ImGui_ImplAndroid_HandleTouchEvent((AInputEvent *)event, {scaleX, scaleY});
    old_input(event, exAb, exAc);
}


int (*old_getWidth)(ANativeWindow* window);
int hook_getWidth(ANativeWindow* window) {
	egl.screenWidth = old_getWidth(window);
	return old_getWidth(window);
}

int (*old_getHeight)(ANativeWindow* window);
int hook_getHeight(ANativeWindow* window) {
	egl.screenHeight = old_getHeight(window);
	return old_getHeight(window);
}

float density = -1;
ImFont *font;

static bool fontsLoaded = false; // ประกาศแถวบนสุด หรือ global

static ImFont* fontENG = nullptr;
static ImFont* fontTH = nullptr;

void LoadFonts() {
    if (fontsLoaded) return;
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;

    fontENG = io.Fonts->AddFontFromMemoryTTF((void*)SYNZ_data, SYNZ_size, 27.0f, &cfg, io.Fonts->GetGlyphRangesDefault());
    fontTH = io.Fonts->AddFontFromMemoryTTF((void*)SYNZ1_data, SYNZ1_size, 27.0f, &cfg, io.Fonts->GetGlyphRangesThai());
    
    io.Fonts->Build();
    io.FontDefault = fontTH;
    fontsLoaded = true;
}


inline static bool g_IsSetup = false;
inline int prevWidth, prevHeight;
ImFontConfig config;

void SetupImgui() {
    if (g_IsSetup) return; // ป้องกันการ Setup ซ้ำที่ทำให้แลค

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // ตั้งค่าหน้าจอครั้งเดียว
    io.DisplaySize = ImVec2((float)egl.width, (float)egl.height);
    
    ImGui::StyleColorsDark();
    ImGuiStyle* style = &ImGui::GetStyle();
    
    // ตั้งค่า Scaling ครั้งเดียว ไม่ต้องเบิ้ล
    style->WindowRounding = 4.0f;
    style->FrameRounding = 2.0f;
    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style->FramePadding = ImVec2(8.0f, 6.0f);
    style->ScaleAllSizes(3.0f); 
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui_ImplAndroid_Init(NULL);
    
    LoadFonts(); 
    
    g_IsSetup = true;
}

struct UnityEngine_Vector2_Fields {
float x;
float y;
};

struct UnityEngine_Vector2_o {
UnityEngine_Vector2_Fields fields;
};

enum TouchPhase {
Began = 0,
Moved = 1,
Stationary = 2,
Ended = 3,
Canceled = 4
};

struct UnityEngine_Touch_Fields {
int32_t m_FingerId;
struct UnityEngine_Vector2_o m_Position;
struct UnityEngine_Vector2_o m_RawPosition;
struct UnityEngine_Vector2_o m_PositionDelta;
float m_TimeDelta;
int32_t m_TapCount;
int32_t m_Phase;
int32_t m_Type;
float m_Pressure;
float m_maximumPossiblePressure;
float m_Radius;
float m_RadiusVariance;
float m_AltitudeAngle;
float m_AzimuthAngle;
};




