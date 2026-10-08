#include <imports.h>
#include <imguiconfig.h>
#include "ESP.h"
#include <string>
#include <chrono>
#include "Toggle.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "icon.h" 
#include <GLES3/gl3.h> /
#include <vector>

#ifndef OBFUSCATE
#define OBFUSCATE(str) str
#endif

//BS4484 YOU CAN DELETE CREDIT I DONT CARE

bool LoadTextureFromMemory(const unsigned char* data, size_t data_size, GLuint* out_texture) {
    int width, height;
    unsigned char* image_data = stbi_load_from_memory(data, data_size, &width, &height, NULL, 4);
    if (image_data == NULL) return false;

    glGenTextures(1, out_texture);
    glBindTexture(GL_TEXTURE_2D, *out_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);
    return true;
}

bool BypassTest = false;
int currentLanguage = 0;
bool checked = false;
bool init = true;
static auto lastTime = std::chrono::high_resolution_clock::now();
#include "themesyn.h"


static bool g_isMenuVisible = true;

//=========================================================

#include <functional> 

struct Notification {
    std::string title;
    std::string message;
    ImVec4 color;
    float progress = 0.0f;
    float alpha = 0.0f; 
    bool is_done = false;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
};

std::vector<Notification> toast_notifications;

void show_toast(std::string title, std::string msg, ImVec4 color) {
    Notification n;
    n.title = title;
    n.message = msg;
    n.color = color;
    n.start_time = std::chrono::steady_clock::now();
    toast_notifications.push_back(n);
}

void RenderLibToasts() {
    if (toast_notifications.empty()) return;
    for (auto it = toast_notifications.begin(); it != toast_notifications.end();) {
        if (std::chrono::duration<float>(std::chrono::steady_clock::now() - it->start_time).count() >= 5.0f) {
            it = toast_notifications.erase(it);
        } else {
            ++it;
        }
    }
    if (toast_notifications.empty()) return;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    float x_pos = io.DisplaySize.x - 20.0f, y_pos = 45.0f, height = 30.0f;
    int display_count = std::min((int)toast_notifications.size(), 3);

    for (int i = 0; i < display_count; i++) {
        auto& toast = toast_notifications[i];
        float elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - toast.start_time).count();
        float duration = 5.0f, anim_time = 0.25f;
        toast.progress = elapsed / duration;

        if (elapsed < anim_time) {
            float t = elapsed / anim_time;
            toast.alpha = t * (2.0f - t);
        } else if (elapsed > (duration - anim_time)) {
            float t = (duration - elapsed) / anim_time;
            toast.alpha = t * (2.0f - t);
        } else {
            toast.alpha = 1.0f;
        }

        std::string status = " [" + toast.message + "] ";
        float title_w = ImGui::CalcTextSize(toast.title.c_str()).x;
        float status_w = ImGui::CalcTextSize(status.c_str()).x;
        float width = title_w + status_w + 25.0f;
        float slide = (1.0f - toast.alpha) * 25.0f;
        
        ImVec2 p_min(x_pos - width + slide, y_pos), p_max(x_pos + slide, y_pos + height);

        ImU32 col_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.95f * toast.alpha));
        ImU32 col_red = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85f, 0.0f, 0.0f, toast.alpha));
        ImU32 col_white = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, toast.alpha));

        draw_list->AddRectFilled(p_min, p_max, col_bg);
        draw_list->AddRect(p_min, p_max, ImGui::ColorConvertFloat4ToU32(ImVec4(0.85f, 0.0f, 0.0f, 0.5f * toast.alpha)), 0.0f, 0, 1.0f);

        float text_y = p_min.y + (height - ImGui::GetTextLineHeight()) * 0.5f;
        draw_list->AddText(ImVec2(p_min.x + 10.0f, text_y), col_white, toast.title.c_str());
        draw_list->AddText(ImVec2(p_max.x - status_w - 10.0f, text_y), col_red, status.c_str());

        draw_list->AddRectFilled(ImVec2(p_min.x, p_max.y - 2.0f), ImVec2(p_min.x + (width * (1.0f - toast.progress)), p_max.y), col_red);

        y_pos += (height + 6.0f);
    }
}

void DrawFloatingToggle(const char* window_id, const char* label, bool* v) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | 
                             ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoScrollbar | 
                             ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::Begin(window_id, NULL, flags);
    ImGui::Text(label); 
    ToggleSYNF("", v); 
    ImGui::End();
    ImGui::PopStyleVar();
}

bool show_Autokill = false;
bool show_Coverkill = false;
bool show_SpeedTime = false;
bool show_FlyTire = false;
bool show_FlyHover = false;
bool show_Hitfly = false;
bool show_AutoFire = false;
bool show_Autokill2 = false;
bool show_AutoFly = false;
bool show_Autokill3 = false;
bool show_TPPLAYER = false;
bool show_TPMAPMARK = false;
bool show_TPKILL = false;
bool show_StopTime = false;
bool show_AutoFly2 = false;
bool show_AutoSwap = false;
bool show_FlyTelePLUS = false;
bool show_AutoTeleportMark = false;
bool show_AutoTeleportPlayer = false;
#include <unistd.h>

std::string GetConfigPath() {
    char cmdline[256] = {0};
    FILE* fp = fopen("/proc/self/cmdline", "r");
    if (fp) {
        fgets(cmdline, sizeof(cmdline) - 1, fp);
        fclose(fp);
    }
    std::string pkgName(cmdline);
    if (pkgName.find("com.dts.freefiremax") != std::string::npos) {
        return "/storage/emulated/0/Android/data/com.dts.freefiremax/files/bs_config.cfg";
    }
    return "/storage/emulated/0/Android/data/com.dts.freefireth/files/bs_config.cfg";
}

int Language = 0;
int selectedTheme = 1;
int current_delay_idx = 0;
int current_delay_idx1 = 1;

void SaveConfig() {
    std::ofstream file(GetConfigPath());
    if (!file.is_open()) return;

    // ไล่บันทึกตัวแปรทั้งหมดเรียงตามลำดับ
    file << Language << "\n" << selectedTheme << "\n";
    file << EnableEsp << "\n" << EnableFunction << "\n" << chams << "\n";
    file << EspLine << "\n" << EspBox2 << "\n" << ESPInfo << "\n" << EnableTeamESP << "\n";
    file << EspVehicle << "\n" << EspBox4 << "\n" << EspBox3D << "\n" << ESPArrow << "\n";
    file << EspGrenade << "\n" << EspFire << "\n" << EspFireBlue << "\n";
    file << outline << "\n" << wireframe << "\n" << glow << "\n";
    file << r << "\n" << g << "\n" << b << "\n";
    file << AimVisible << "\n" << Fov_Aim << "\n" << ShowFOV << "\n";
    file << AimbotLock << "\n" << AimbotFiring << "\n" << AimbotScope << "\n";
    file << AimAutofire << "\n" << SilentAimPro << "\n";
    file << AimkillStart << "\n" << AimkillStart2 << "\n" << Telekill << "\n";
    file << Underkill << "\n" << PullEnemyV1 << "\n" << EnemyLerp << "\n" << EnableYLerp << "\n";
    file << AutoSwap << "\n" << StartRapidfire << "\n" << NoReloadStart << "\n";
    file << fastswitch << "\n" << Norecoil << "\n" << InfiniteRange << "\n";
    file << current_delay_idx << "\n" << current_delay_idx1 << "\n";
    file << GroundHack << "\n" << GravityHack << "\n" << SpeedRun << "\n";
    file << SpeedJoys << "\n" << SpeedTime << "\n" << Time0 << "\n";
    file << Menu_AutoFlyForward << "\n" << GUI_FlySpeedXZ << "\n" << GUI_FlySpeedY << "\n";
    file << ShowBoard << "\n" << TestGilde2 << "\n" << TestGilde1 << "\n";
    file << SuperJump << "\n" << FlyHover << "\n" << FlyTire << "\n" << FlyTireV2 << "\n";
    file << FlyCarStart << "\n" << Fly_Height << "\n" << Fly_Speed << "\n";
    file << TPPlayer << "\n" << TeleMark << "\n" << AfkNobot << "\n";
    file << StartAutoRivive << "\n" << StartAutoExecute << "\n";
    file << EnableFov << "\n" << EnableNightMode << "\n" << SmoothFps << "\n" << highfps << "\n";
    file << EnableResetGuest << "\n" << UnlockTraining << "\n" << show_AutoTeleportPlayer << "\n";
    file << show_Autokill << "\n" << show_Autokill3 << "\n" << show_AutoFire << "\n";
    file << show_Coverkill << "\n" << show_SpeedTime << "\n" << show_StopTime << "\n";
    file << show_Autokill2 << "\n" << show_AutoFly << "\n" << show_FlyTire << "\n";
    file << show_FlyHover << "\n" << show_Hitfly << "\n" << show_TPPLAYER << "\n";
    file << show_TPMAPMARK << "\n" << show_TPKILL << "\n";
    file << LogSend << "\n" << enableClassLog << "\n" << current_selection << "\n";
    file << ForceState << "\n" << selectedState << "\n" << selectedPose << "\n";
	file << Hidedamage << "\n" << InvisibleAWM << "\n" << show_AutoFly2 << "\n";
	file << EspHealthBar << "\n" << EspDistance << "\n" << EspName << "\n";
	file << show_AutoSwap << "\n" << EspAlert << "\n" << show_AutoTeleportMark << "\n";
    file.close();
}
void LoadConfig() {
	std::ifstream file(GetConfigPath());
    if (!file.is_open()) return;

    file >> Language >> selectedTheme;
    file >> EnableEsp >> EnableFunction >> chams;
    file >> EspLine >> EspBox2 >> ESPInfo >> EnableTeamESP;
    file >> EspVehicle >> EspBox4 >> EspBox3D >> ESPArrow;
    file >> EspGrenade >> EspFire >> EspFireBlue;
    file >> outline >> wireframe >> glow;
    file >> r >> g >> b;
    file >> AimVisible >> Fov_Aim >> ShowFOV;
    file >> AimbotLock >> AimbotFiring >> AimbotScope;
    file >> AimAutofire >> SilentAimPro;
    file >> AimkillStart >> AimkillStart2 >> Telekill;
    file >> Underkill >> PullEnemyV1 >> EnemyLerp >> EnableYLerp;
    file >> AutoSwap >> StartRapidfire >> NoReloadStart;
    file >> fastswitch >> Norecoil >> InfiniteRange;
    file >> current_delay_idx >> current_delay_idx1;
    file >> GroundHack >> GravityHack >> SpeedRun;
    file >> SpeedJoys >> SpeedTime >> Time0;
    file >> Menu_AutoFlyForward >> GUI_FlySpeedXZ >> GUI_FlySpeedY;
    file >> ShowBoard >> TestGilde2 >> TestGilde1;
    file >> SuperJump >> FlyHover >> FlyTire >> FlyTireV2;
    file >> FlyCarStart >> Fly_Height >> Fly_Speed;
    file >> TPPlayer >> TeleMark >> AfkNobot;
    file >> StartAutoRivive >> StartAutoExecute;
    file >> EnableFov >> EnableNightMode >> SmoothFps >> highfps;
    file >> EnableResetGuest >> UnlockTraining >> show_AutoTeleportPlayer;
    file >> show_Autokill >> show_Autokill3 >> show_AutoFire;
    file >> show_Coverkill >> show_SpeedTime >> show_StopTime;
    file >> show_Autokill2 >> show_AutoFly >> show_FlyTire;
    file >> show_FlyHover >> show_Hitfly >> show_TPPLAYER;
    file >> show_TPMAPMARK >> show_TPKILL;
    file >> LogSend >> enableClassLog >> current_selection;
    file >> ForceState >> selectedState >> selectedPose;
	file >> Hidedamage >> InvisibleAWM >> show_AutoFly2;
	file >> EspHealthBar >> EspDistance >> EspName;
	file >> show_AutoSwap >> EspAlert >> show_AutoTeleportMark;
    file.close();

    if (current_delay_idx == 0) SilentAimDelay = 0.02f;
    else if (current_delay_idx == 1) SilentAimDelay = 0.04f;
    else if (current_delay_idx == 2) SilentAimDelay = 0.08f;

    if (current_delay_idx1 == 0) delayweapon = 0.02f;
    else if (current_delay_idx1 == 1) delayweapon = 0.04f;
    else if (current_delay_idx1 == 2) delayweapon = 0.08f;

    if (current_selection == 0) ForcePhysXState = -1; 
    else ForcePhysXState = current_selection;
}
bool EnableAutoSave = false;

struct ConfigState {
    int Language, selectedTheme, current_delay_idx, current_delay_idx1, current_selection;
    bool EnableEsp, EnableFunction, chams, EspLine, EspBox2, ESPInfo, EnableTeamESP;
    bool EspVehicle, EspBox4, EspBox3D, ESPArrow, EspGrenade, EspFire, EspFireBlue;
    bool outline, wireframe, glow, rainbow;
    float r, g, b;
    bool AimVisible;
    float Fov_Aim;
    bool ShowFOV, AimbotLock, AimbotFiring, AimbotScope, AimAutofire, SilentAimPro;
    bool AimkillStart, AimkillStart2, Telekill, Underkill, PullEnemyV1, EnemyLerp, EnableYLerp;
    bool AutoSwap, StartRapidfire, NoReloadStart, fastswitch, Norecoil, InfiniteRange;
    bool GroundHack, GravityHack, NofallDmg, SpeedRun, SpeedJoys, SpeedTime;
    float Time0, TimeSpeed;
    bool Menu_AutoFlyForward;
    float GUI_FlySpeedXZ, GUI_FlySpeedY;
    bool ShowBoard, TestGilde2, TestGilde1, SuperJump, FlyHover, FlyTire, FlyTireV2;
    bool FlyCarStart;
    float Fly_Height, Fly_Speed;
    bool TPPlayer, TeleMark, AfkNobot, StartAutoRivive, StartAutoExecute;
    bool EnableFov, EnableNightMode, SmoothFps, highfps, EnableResetGuest, UnlockTraining;
    bool show_Autokill, show_Autokill3, show_AutoFire, show_Coverkill, show_SpeedTime, show_StopTime;
    bool show_Autokill2, show_FlyTire, show_FlyHover, show_Hitfly, show_TPPLAYER;
    bool show_TPMAPMARK, show_TPKILL, LogSend, enableClassLog, ForceState;
    int selectedState, selectedPose;
    bool Hidedamage, InvisibleAWM, show_AutoFly2, EspHealthBar, EspDistance, EspName;
    bool show_AutoSwap, EspAlert, show_AutoTeleportMark, FlyAdmin, show_AutoTeleportPlayer;
    float FlyAdminHeight, Fly_AdminSpeed;
} lastConfigState;

ConfigState CaptureConfigState() {
    ConfigState s;
    s.Language = Language; s.selectedTheme = selectedTheme; s.current_delay_idx = current_delay_idx; s.current_delay_idx1 = current_delay_idx1; s.current_selection = current_selection;
    s.EnableEsp = EnableEsp; s.EnableFunction = EnableFunction; s.chams = chams; s.EspLine = EspLine; s.EspBox2 = EspBox2; s.ESPInfo = ESPInfo; s.EnableTeamESP = EnableTeamESP;
    s.EspVehicle = EspVehicle; s.EspBox4 = EspBox4; s.EspBox3D = EspBox3D; s.ESPArrow = ESPArrow; s.EspGrenade = EspGrenade; s.EspFire = EspFire; s.EspFireBlue = EspFireBlue;
    s.outline = outline; s.wireframe = wireframe; s.glow = glow; s.rainbow = rainbow;
    s.r = r; s.g = g; s.b = b;
	s.show_AutoTeleportPlayer = show_AutoTeleportPlayer;
    s.AimVisible = AimVisible; s.Fov_Aim = Fov_Aim; s.ShowFOV = ShowFOV; s.AimbotLock = AimbotLock; s.AimbotFiring = AimbotFiring; s.AimbotScope = AimbotScope; s.AimAutofire = AimAutofire; s.SilentAimPro = SilentAimPro;
    s.AimkillStart = AimkillStart; s.AimkillStart2 = AimkillStart2; s.Telekill = Telekill; s.Underkill = Underkill; s.PullEnemyV1 = PullEnemyV1; s.EnemyLerp = EnemyLerp; s.EnableYLerp = EnableYLerp;
    s.AutoSwap = AutoSwap; s.StartRapidfire = StartRapidfire; s.NoReloadStart = NoReloadStart; s.fastswitch = fastswitch; s.Norecoil = Norecoil; s.InfiniteRange = InfiniteRange;
    s.GroundHack = GroundHack; s.GravityHack = GravityHack; s.NofallDmg = NofallDmg; s.SpeedRun = SpeedRun; s.SpeedJoys = SpeedJoys; s.SpeedTime = SpeedTime;
    s.Time0 = Time0; s.TimeSpeed = TimeSpeed; s.Menu_AutoFlyForward = Menu_AutoFlyForward; s.GUI_FlySpeedXZ = GUI_FlySpeedXZ; s.GUI_FlySpeedY = GUI_FlySpeedY;
    s.ShowBoard = ShowBoard; s.TestGilde2 = TestGilde2; s.TestGilde1 = TestGilde1; s.SuperJump = SuperJump; s.FlyHover = FlyHover; s.FlyTire = FlyTire; s.FlyTireV2 = FlyTireV2;
    s.FlyCarStart = FlyCarStart; s.Fly_Height = Fly_Height; s.Fly_Speed = Fly_Speed;
    s.TPPlayer = TPPlayer; s.TeleMark = TeleMark; s.AfkNobot = AfkNobot; s.StartAutoRivive = StartAutoRivive; s.StartAutoExecute = StartAutoExecute;
    s.EnableFov = EnableFov; s.EnableNightMode = EnableNightMode; s.SmoothFps = SmoothFps; s.highfps = highfps; s.EnableResetGuest = EnableResetGuest; s.UnlockTraining = UnlockTraining;
    s.show_Autokill = show_Autokill; s.show_Autokill3 = show_Autokill3; s.show_AutoFire = show_AutoFire; s.show_Coverkill = show_Coverkill; s.show_SpeedTime = show_SpeedTime; s.show_StopTime = show_StopTime;
    s.show_Autokill2 = show_Autokill2; s.show_FlyTire = show_FlyTire; s.show_FlyHover = show_FlyHover; s.show_Hitfly = show_Hitfly; s.show_TPPLAYER = show_TPPLAYER;
    s.show_TPMAPMARK = show_TPMAPMARK; s.show_TPKILL = show_TPKILL; s.LogSend = LogSend; s.enableClassLog = enableClassLog; s.ForceState = ForceState;
    s.selectedState = selectedState; s.selectedPose = selectedPose; s.Hidedamage = Hidedamage; s.InvisibleAWM = InvisibleAWM; s.show_AutoFly2 = show_AutoFly2; s.EspHealthBar = EspHealthBar; s.EspDistance = EspDistance; s.EspName = EspName;
    s.show_AutoSwap = show_AutoSwap; s.EspAlert = EspAlert; s.show_AutoTeleportMark = show_AutoTeleportMark; s.FlyAdmin = FlyAdmin; s.FlyAdminHeight = FlyAdminHeight; s.Fly_AdminSpeed = Fly_AdminSpeed;
    return s;
}

bool AreStatesDifferent(const ConfigState& a, const ConfigState& b) {
    return std::memcmp(&a, &b, sizeof(ConfigState)) != 0;
}

void menu()
{
    RenderLibToasts();
    static GLuint icon_texture_id = 0;
    static bool is_loaded = false;
    if (!is_loaded) {
        is_loaded = LoadTextureFromMemory(logo_png, logo_png_len, &icon_texture_id);
    }
    if (!g_isMenuVisible) 
    {
        ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiCond_FirstUseEver);
        ImGui::Begin("##FloatingButton", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

        #define ImGuiMouseButton_Left 0

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); 
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);  
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); 

        bool button_clicked = false;

        if (is_loaded) {
            #if IMGUI_VERSION_NUM >= 18900
            button_clicked = ImGui::ImageButton("##ToggleBtn", (ImTextureID)(intptr_t)icon_texture_id, ImVec2(110, 110), ImVec2(0,0), ImVec2(1,1), ImVec4(0,0,0,0));
            #else
            button_clicked = ImGui::ImageButton((ImTextureID)(intptr_t)icon_texture_id, ImVec2(110, 110), ImVec2(0,0), ImVec2(1,1), -1, ImVec4(0,0,0,0));
            #endif
        } else {
            button_clicked = ImGui::Button("OPEN", ImVec2(80, 80));
        }

        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            ImVec2 pos = ImGui::GetWindowPos();
            ImGui::SetWindowPos(ImVec2(pos.x + delta.x, pos.y + delta.y));
        }

        if (button_clicked && !ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            g_isMenuVisible = true; 
        }

        ImGui::PopStyleVar(3);  
        ImGui::PopStyleColor(4); 
        ImGui::End();
    }

    if (!checked) { checked = true; }
	static bool first_load_done = false;
    if (!first_load_done) {
        //LoadConfig(); // โหลดค่าให้อัตโนมัติครั้งแรกตอนเข้าเกม
        lastConfigState = CaptureConfigState();
        first_load_done = true;
    }

    if (EnableAutoSave) {
        ConfigState currentState = CaptureConfigState();
        if (AreStatesDifferent(lastConfigState, currentState)) {
            SaveConfig(); // บันทึกอัตโนมัติเมื่อค่ามีการเปลี่ยนแปลง
            lastConfigState = currentState;
        }
    }

    if (!init) return;

    auto T = [&](const char* en, const char* th) { return (Language == 0) ? en : th; };
    const char* lang_names[] = { "ENGLISH", "ไทย" }; 
    const char* combo_label = (Language == 0) ? "Language" : "ภาษา";

    static bool IsBall = true; static float ANIM_SPEED = 0.25f; static float Velua = IsBall ? 0.0f : 1.0f;
    Velua = ImClamp(Velua + (ImGui::GetIO().DeltaTime / ANIM_SPEED) * (IsBall ? 1.0f : -1.0f), 0.0f, 1.0f);
    ImVec4 startColor = ImColor(0.00f, 0.00f, 0.00f, 1.00f); 
    ImVec4 endColor = ImColor(0.05f, 0.00f, 0.00f, 1.00f);   
    ImVec4 currentColor = ImLerp(startColor, endColor, Velua);
    if (Velua > 0.0f) { ImGui::PushStyleColor(ImGuiCol_WindowBg, currentColor); }

    if (selectedTheme == 0) { ApplyGreyTheme(); } 
    else if (selectedTheme == 1) { ApplyRedTheme(); } 
    else if (selectedTheme == 2) { ApplyBlueTheme(); }
    else if (selectedTheme == 3) { ApplyGreenTheme(); }
    const char* theme_names[] = { "Grey Theme", "Red Theme", "Blue Theme", "Green Theme" };

    if (g_isMenuVisible) 
    {
        ImGui::SetNextWindowSize(ImVec2(580, 520), ImGuiCond_Always); 
        ImGui::Begin("BS4484 V2.4", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
        
        // =====================ฝั่งซ้าย: SIDEBAR SYSTEM =====================
        ImGui::BeginChild("##Sidebar", ImVec2(140, 0), true, ImGuiWindowFlags_NoScrollbar);
 
        ImGui::Spacing();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);
        if (is_loaded) {
            ImGui::Image((ImTextureID)(intptr_t)icon_texture_id, ImVec2(80, 80));
            ImGui::Spacing();
        }
        ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), "BS4484 V2.4");
        ImGui::Separator();
        ImGui::Spacing();
        static int active_tab = 0;
        const char* tab_names[] = { "MAIN", "VISUAL", "COMBAT", "MOVEMENT", "MISC/VEH" };
        
        for (int i = 0; i < 5; i++)
        {
            bool is_selected = (active_tab == i);
            if (is_selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.60f, 0.00f, 0.00f, 1.00f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.00f, 0.00f, 1.00f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.08f, 0.08f, 0.08f, 1.00f)); 
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.14f, 0.14f, 0.14f, 1.00f));
            }

            if (ImGui::Button(tab_names[i], ImVec2(124, 40))) {
                active_tab = i;
            }
            ImGui::PopStyleColor(2);
            ImGui::Spacing();
        }

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 48.0f); 
        ImGui::Separator();
        if (ImGui::Button("CLOSE", ImVec2(124, 30))) {
            g_isMenuVisible = false;
        }
		
        ImGui::EndChild();

        // =====================ฝั่งขวา: CONTENT AREA =====================
        ImGui::SameLine();
        ImGui::BeginChild("##ContentArea", ImVec2(0, 0), true);

        auto RenderCustomHeader = [](const char* label) {
            ImVec2 p = ImGui::GetCursorScreenPos();
            float w = ImGui::GetContentRegionAvail().x;
            float h = 26.0f;
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + w, p.y + h), ImGui::GetColorU32(ImGuiCol_WindowBg), 0.0f);
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + w, p.y + h), ImColor(15, 15, 15, 255), 0.0f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
            ImGui::TextColored(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive], "%s", label);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
        };

        if (active_tab == 0) // ===================== TAB: MAIN =====================
        {
            RenderCustomHeader("MAIN FUNCTION");
			ToggleSYN(T("LOGIN INDIA", "ล็อคอิน อินเดีย"), &IndiaLogin);
            ToggleSYN(T("ENABLE ESP", "ใช้งานมองเส้น"), &EnableEsp);
            ToggleSYN(T("ENABLE FUNCTION", "ใช้งานฟังชั่น"), &EnableFunction);
 			
            ImGui::Separator();
            RenderCustomHeader(T("CONFIGURATION FUNCTION", "ระบบบันทึกตั้งค่า"));
            ToggleSYN(T("AUTOSAVE FUNCTION", "บันทึกอัตโนมัติ"), &EnableAutoSave); 
            if (ImGui::Button(T("SAVE FUNCTION", "บันทึกตั้งค่า"), ImVec2(-1, 32))) { SaveConfig(); }
            if (ImGui::Button(T("LOAD FUNCTION", "โหลดตั้งค่า"), ImVec2(-1, 32))) { LoadConfig(); }

            ImGui::Separator();
			RenderCustomHeader("INFORMATION");
			#if defined(__aarch64__)
    			ImGui::Text("Architecture : FREE FIRE 64-bit [ARM64]");
			#else
    			ImGui::Text("Architecture : FREE FIRE 32-bit [ARM]");
			#endif
			if (ImGui::Button(T("TELEGRAM CHANNEL", "แชนแนลเทเลแกรม"), ImVec2(-1, 32))) { 
    			OpenURL(il2cpp_string_new("https://t.me/BloodShotTH")); 
			}
			
            RenderCustomHeader(T("MENU SETTINGS", "ตั้งค่าธีมสี"));
			const char* theme_names[] = { "Grey Theme", "Red Theme" };
			if (ImGui::Combo(T("THEME", "รูปแบบธีม"), &selectedTheme, theme_names, IM_ARRAYSIZE(theme_names))) {}
			ImGui::Combo(combo_label, &Language, lang_names, IM_ARRAYSIZE(lang_names));
        }
        else if (active_tab == 1) // ===================== TAB: VISUAL =====================
        {
			RenderCustomHeader("ESP OBJECT");
			ToggleSYN(T("ESP GRENADE", "ESP เส้นระเบิด"), &EspGrenade);
			ToggleSYN(T("ESP FIRE", "ESP เส้นกระสุน"), &EspFire);
			ToggleSYN(T("ESP FIRE BLUE", "ESP เส้นกระสุนฟ้า"), &EspFireBlue);
			ToggleSYN(T("ESP ALERT", "ESP แจ้งเตือนศัตร"), &EspAlert);
			
			RenderCustomHeader("ESP DRAW");
            ToggleSYN(T("ESP LINE", "ESP เส้น"), &EspLine);
            ToggleSYN(T("ESP BOX", "ESP กรอบ"), &EspBox2);
            ToggleSYN(T("ESP HEALTH", "ESP เลือด"), &EspHealthBar);
			ToggleSYN(T("ESP DISTANCE", "ESP ระยะทาง"), &EspDistance);
			ToggleSYN(T("ESP NAME", "ESP ชื่อ"), &EspName);
            ToggleSYN(T("ESP TEAM", "ESP ทีม"), &EnableTeamESP);
            ToggleSYN(T("ESP VEHICLE", "ESP ยานพาหนะ"), &EspVehicle);
			ToggleSYN(T("ESP BOXFIELD", "ESP กล่องทึบ"), &EspBox4);
        	ToggleSYN(T("ESP BOX3D", "ESP กล่องสามมิติ"), &EspBox3D);
        	ToggleSYN(T("ESP ARROW", "ESP ศร"), &ESPArrow);
			ImGui::ColorEdit4("Enemy Color", EnemyColor);
			ImGui::ColorEdit4("Team Color", TeamColor);
			ImGui::ColorEdit4("Vehicle Color", VehColor);

			RenderCustomHeader("CHAMS CONFIG");
			ToggleSYN(T("OUTLINE", "เส้นขอบ"), &outline);
			ToggleSYN(T("WIREFRAME", "ลวดลายลวด"), &wireframe);
			ToggleSYN(T("GLOW", "เรืองแสง"), &glow);
			ToggleSYN(T("SHADING", "เงาพื้นผิว"), &shading);
			ToggleSYN(T("RAINBOW", "สายรุ้ง"), &rainbow);

			static float chamsColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			chamsColor[0] = r / 255.0f;
			chamsColor[1] = g / 255.0f;
			chamsColor[2] = b / 255.0f;
			chamsColor[3] = a / 255.0f;

			if (ImGui::Button(T("CHAMS COLOR", "เปลี่ยนสี Chams"), ImVec2(120, 30))) {
			    ImGui::OpenPopup("ColorPickerPopup");
			}
			ImGui::SameLine();
			ImGui::ColorButton("##color_preview", ImVec4(chamsColor[0], chamsColor[1], chamsColor[2], chamsColor[3]), ImGuiColorEditFlags_NoTooltip, ImVec2(30, 30));

			ImGui::SameLine();
			ImGui::Text("R:%.0f G:%.0f B:%.0f A:%.0f", r, g, b, a);

			if (ImGui::BeginPopup("ColorPickerPopup")) {
			    ImGui::Text("Chams Color Picker");
			    ImGui::Separator();

			    if (ImGui::ColorPicker4("##picker", chamsColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB)) {
			        r = chamsColor[0] * 255.0f;
			        g = chamsColor[1] * 255.0f;
			        b = chamsColor[2] * 255.0f;
			        a = chamsColor[3] * 255.0f;
			    }
    
			    ImGui::EndPopup();
			}
			BSSliderFloat(T("ALPHA", "ความโปร่งใส"), (float*)&a, 0.0f, 255.0f);
        }
        else if (active_tab == 2) // ===================== TAB: COMBAT =====================
        {
            RenderCustomHeader("AIM CONFIG");
            ToggleSYN(T("AIM VISIBLE", "เช็คกำแพง"), &AimVisible);
            BSSliderFloat(T("AIM FOV", "มุม FOV"), &Fov_Aim, 0, 180);
            ToggleSYN(T("SHOW FOV", "แสดง FOV"), &ShowFOV);
			
            RenderCustomHeader("AIM BOT");
            ToggleSYN(T("AIMBOT LOCK", "ล็อกตลอดเวลา"), &AimbotLock);
            ToggleSYN(T("AIMBOT FIRING", "ล็อกตอนยิง"), &AimbotFiring);
            ToggleSYN(T("AIMBOT SCOPE", "ล็อกตอนเล็งสโคป"), &AimbotScope);

			const char* btnText = AimTarget ? "AIM TARGET: BODY" : "AIM TARGET: HEAD";
			ImVec4 btnColor = AimTarget ? ImVec4(0.2f, 0.6f, 0.2f, 1.0f) : ImVec4(0.6f, 0.2f, 0.2f, 1.0f); // เปิด = เขียว, ปิด = แดง

			ImGui::PushStyleColor(ImGuiCol_Button, btnColor);
			if (ImGui::Button(btnText, ImVec2(-1, 30.0f))) { 
			    AimTarget = !AimTarget;
			}
			ImGui::PopStyleColor();
			
            RenderCustomHeader("BRUTAL KILL");
			ToggleSYN(T("AIM SILENT", "เล็งกระสุนติดตาม"), &SilentAimPro);
            ToggleSYN(T("AIM KILL (FLOATING)", "ฆ่าออโต้ (เมนูลอย)"), &show_Autokill);
			ToggleSYN(T("AUTO FIRE (FLOATING)", "เล็งยิงออโต้ (เมนูลอย)"), &show_AutoFire);
			ToggleSYN(T("MASS KILL", "ดึงศัตรู"), &PullEnemyV1);
            ToggleSYN(T("COVER KILL", "ศัตรูขยับไปมา"), &EnemyLerp);
			if (EnemyLerp) { ImGui::Indent(); ToggleSYN(T("ENABLE Y-AXIS SHAKE", "ขยับแกน Y ด้วย"), &EnableYLerp); ImGui::Unindent(); }

            RenderCustomHeader("WEAPON");
			ToggleSYN(T("HIDE DAMAGE", "ซ่อนดาเมจ"), &Hidedamage);
			ToggleSYN(T("AUTO SWAPWEAPON", "สลับปืนอัตโนมัติ"), &AutoSwap);
			ToggleSYN(T("RAPID FIRE", "ยิงรัว"), &StartRapidfire);
			ToggleSYN(T("NO RELOAD", "ไม่มีรีโหลด"), &NoReloadStart);
        	ToggleSYN(T("FAST SWAP", "เปลี่ยนอาวุธเร็ว"), &fastswitch);
        	ToggleSYN(T("NO RECOIL", "ไม่มีรีคอล์ย"), &Norecoil);
			
			RenderCustomHeader("CONFIG");
            const char* delay_items[] = { "2ms", "4ms", "8ms"};
            ImGui::Combo("AUTOKILL DELAY", &current_delay_idx, delay_items, IM_ARRAYSIZE(delay_items));
            if (current_delay_idx == 0) SilentAimDelay = 0.02f;
            else if (current_delay_idx == 1) SilentAimDelay = 0.04f;
            else if (current_delay_idx == 2) SilentAimDelay = 0.08f;

			const char* delay_items1[] = { "2ms", "4ms", "8ms"};
			ImGui::Combo("AUTOFIRE DELAY", &current_delay_idx1, delay_items1, IM_ARRAYSIZE(delay_items1));
			if (current_delay_idx1 == 0) delayweapon = 0.02f;
			else if (current_delay_idx1 == 1) delayweapon = 0.04f;
			else if (current_delay_idx1 == 2) delayweapon = 0.08f;
        }
        else if (active_tab == 3) // ===================== TAB: MOVEMENT =====================
        {
            RenderCustomHeader("MOVEMENT");			
			ToggleSYN(T("GROUND HACK", "ยืนบนพื้นดินตลอด"), &GroundHack);
			ToggleSYN(T("NO GRAVITY ", "ไม่มีแรงโน้มถ่วง"), &GravityHack);
			ToggleSYN(T("NO FALL DAMAGE", "ตกที่สูงเลือดไม่ลด"), &NofallDmg);
            ToggleSYN(T("SUPER JUMP (FLOATING)", "กระโดดสูง (เมนูลอย)"), &show_Autokill2);

            RenderCustomHeader("SPEED HACK");
            ToggleSYN(T("SPEED RUN", "วิ่งเร็ว"), &SpeedRun);
			ToggleSYN(T("SPEED JOYS", "เดินเร็ว"), &SpeedJoys);
            ToggleSYN(T("SPEED TIME (FLOATING)", "เวลาเร็ว (เมนูลอย)"), &show_SpeedTime);
			BSSliderFloat(T("TIME SPEED", "ความเร็วเวลา"), &TimeSpeed, 0, 2);
			
            RenderCustomHeader("HITFLY HACK");
		    ToggleSYN(T("START HITFLY (FLOATING)", "บิน กระเด็น (เมนูลอย)"), &show_Hitfly);
            BSSliderFloat(T("HITFLY SPEED", "ความเร็วบิน"), &GUI_FlySpeedXZ, 0, 100, "%.0fx");
			BSSliderFloat(T("HITFLY HEIGHT", "ความสูงบิน"), &GUI_FlySpeedY, 0, 100, "%.0fx");
			
			RenderCustomHeader("FLY VEHICLE");
			ToggleSYN(T("START FLY VEHICLE", "บิน ยานพาหนะ"), &FlyCarStart);
	        BSSliderFloat(T("FLY HEIGHT", "ความสูงบิน"), &Fly_Height, 0, 100, "%.0fx");
            BSSliderFloat(T("FLY SPEED", "ความเร็วบิน"), &Fly_Speed, 0, 100, "%.0fx");

			RenderCustomHeader("FLY PLAYER [ BETA ]");
			ToggleSYN(T("START FLY PLAYER ", "บิน ผู้เล่น "), &FlyAdmin);
			BSSliderFloat(T("FLY HEIGHT ", "ความสูงบิน "), &FlyAdminHeight, 0, 30, "%.0fx");
			BSSliderFloat(T("FLY SPEED ", "ความเร็วบิน "), &Fly_AdminSpeed, 0, 8, "%.0fx");

            RenderCustomHeader("TELEPORT (FLOATING) | Need Trick for teleport");
		    ToggleSYN(T("TELEPORT PLAYER", "เทเลผู้เล่น"), &show_TPPLAYER);
			ToggleSYN(T("TELEPORT MAPMARK", "เทเลแมพมาร์ค"), &show_TPMAPMARK);
			ToggleSYN(T("TELEPORT VEHICLE", "เทเลพอร์ตไปยังรถ"), &teleportToCar);
			//ToggleSYN(T("FIX TELEPORT", "แก้ไขเทเล"), &FixtelePortNew);
			//BSSliderFloat(T("TEST HIEGHT ", "JSJSJEJ "), &testhiegh, 0, 8, "%.0fx");
			ToggleSYN(T("AUTO TELEPORT MAPMARK", "ออโต เทเลแมพมาร์ค"), &show_AutoTeleportMark);
			ToggleSYN(T("AUTO TELEPORT PLAYER", "ออโต เทเลผู้เล่น"), &show_AutoTeleportPlayer);
        }
        else if (active_tab == 4) // ===================== TAB: MISC =====================
        {
            RenderCustomHeader("MISC SETTINGS");
			ToggleSYN(T("CAMERA 90°", "กล้องกว้าง 90°"), &EnableFov);
			ToggleSYN(T("NIGHT SKY", "ท้องฟ้ามืด"), &EnableNightMode);
            ToggleSYN(T("SMOOTH FPS", "สมูท fps"), &SmoothFps);
			ToggleSYN(T("HIGH FPS OPEN", "เปิดให้ใช้ FPS สูง"), &highfps);
            ToggleSYN(T("RESET GUEST", "รีเซ็ตบัญชี"), &EnableResetGuest);
			ToggleSYN(T("UNLOCK LV8", "ปลดล็อค เลเวล8"), &UnlockTraining);
			
			RenderCustomHeader("VEHICLE");
			if (ToggleSYN(T("UNLOCK SPEED", "ปลดล็อคความเร็วรถ"), &isUnlockSpeedEnabled)) {
		    void* CurrentMatch = Curent_Match();
			    if (CurrentMatch != nullptr) {
			        void* local_player = GetLocalPlayer(CurrentMatch);
			        if (local_player != nullptr) {
			            void* myVehicle = GetVehicleIAmIn(local_player);
			            if (myVehicle != nullptr) {
			                if (isUnlockSpeedEnabled) {
			                    TriggerUnLockSpeed(myVehicle);
			                } else {
			                    TriggerLockSpeed(myVehicle);
			                }
			           	}
			        }
			    }
			}
			if (ImGui::Button("BOMBS AWAY!", ImVec2(-1, 30.0f))) {
			    void* CurrentMatch = Curent_Match();
			    if (CurrentMatch != nullptr) { 
			        void* local_player = GetLocalPlayer(CurrentMatch);
			        if (local_player != nullptr) { 
			            void* myVehicle = GetVehicleIAmIn(local_player);
			            if (myVehicle != nullptr) { 
			                TriggerExplosion(myVehicle);
			            }
			        }
			    }
			}
			if (ImGui::Button("KILL VEHICLE", ImVec2(-1, 30.0f))) {
			    void* CurrentMatch = Curent_Match();
			    if (CurrentMatch != nullptr) { 
			        void* local_player = GetLocalPlayer(CurrentMatch);
			        if (local_player != nullptr) { 
			            void* myVehicle = GetVehicleIAmIn(local_player);
			            if (myVehicle != nullptr) { 
			                TriggerVehicleDead(myVehicle);
			            }
			        }
			    }
			}
			/*RenderCustomHeader("TEST ADMIN");
			ToggleSYN(T("log send", "log send"), &LogSend);
			ToggleSYN(T("Save Logsend", "Save Logsend"), &enableClassLog);
			ImGui::Checkbox("Force SPhysXState", &ForceState);
			ImGui::Combo("Select State", &selectedState, state_items, IM_ARRAYSIZE(state_items));
			ImGui::Combo("Select Pose", &selectedPose, pose_items, IM_ARRAYSIZE(pose_items));*/
		}

        ImGui::EndChild(); 
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 25.0f);
        ImGui::SetCursorPosX(160.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        #if defined(__aarch64__)
            ImGui::Text("Free Fire & Free Fire Max OB55 [64-bit]");
        #else
            ImGui::Text("Free Fire & Free Fire Max OB55 [32-bit]");
        #endif
        ImGui::PopStyleColor();

        ImGui::End();
    }

    if (show_Autokill) { DrawFloatingToggle("FloatBox1", "   AUTO KILL       ", &AimkillStart); }
	if (show_Autokill2) { DrawFloatingToggle("FloatBox8", "   SUPER JUMP     ", &SuperJump); }
	if (show_Coverkill) { DrawFloatingToggle("FloatBox2", "   COVER KILL    ", &EnemyLerp); }
	if (show_SpeedTime) { DrawFloatingToggle("FloatBox3", "   SPEED TIME     ", &SpeedTime); }
	if (show_FlyTire) { DrawFloatingToggle("FloatBox4", "   FLY TIRE       ", &FlyTireV2); }
	if (show_FlyHover) { DrawFloatingToggle("FloatBox5", "FLY HOVER    ", &FlyHover); }
    if (show_Hitfly) { DrawFloatingToggle("FloatBox6", "   FLY HITFLY     ", &Menu_AutoFlyForward); }
	if (show_AutoFire) { DrawFloatingToggle("FloatBox7", "   AUTO FIRE       ", &AimAutofire); }
	if (show_AutoFly) { DrawFloatingToggle("FloatBox9", "   AUTO FLYFW      ", &TestGilde2); }
	if (show_TPPLAYER) { DrawFloatingToggle("FloatBox12", "   TELE PLAYER     ", &TPPlayer); }
	if (show_TPMAPMARK) { DrawFloatingToggle("FloatBox13", "   TELE MAPMARK       ", &TeleMark); }
	if (show_AutoFly) { DrawFloatingToggle("FloatBox16", "   FORCE FOLDWING   ", &TestGilde1); }
	if (show_AutoSwap) { DrawFloatingToggle("FloatBox17", "   AUTO SWAP       ", &AutoSwap); }
	if (show_FlyTelePLUS) { DrawFloatingToggle("FloatBox18", "   FLY FIXTELE", &FlyTelePLUS); }
	if (show_AutoTeleportMark) { DrawFloatingToggle("FloatBox19", "   AUTO TELEMARK", &AutoTeleportMark); }
	if (show_AutoTeleportPlayer) { DrawFloatingToggle("FloatBox20", "   AUTO TELEPLAYER", &AutoTeleportPlayer); }
    if (Velua > 0.0f) { ImGui::PopStyleColor(); }
}

