#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include "imgui.h"
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <unity/Vector3.hpp>
#include <Color.h>
#include "obfuscate.h"
#include "unity.h"
#include "Updates.h"
#include "Rect.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <math.h>
#include <stdio.h>
#include "cores.h"
#include "aimbot.h"

bool teleportToCar = false;

bool EspLine = false;
bool ESPInfo = false;
bool EspBox2 = false;
bool ShowFOV = false;
bool EnableTeamESP = false;
bool PullEnemyV1 = false;
bool EspBox3D = false;
bool EspBox4 = false;
bool EspFire = false;
bool EspFireBlue = false;
bool ESPArrow = false;
bool EspVehicle = false;
bool EspName = false;
bool EspHealthBar = false;
bool EspDistance = false;

bool Telekill = false;
bool EnemyLerp = false;
bool EnableYLerp = false;
bool EspGrenade = false;
bool Underkill = false;
static bool isInitialized = false;
static void* lastEnemy = nullptr;
static Vector3 initEnemyPos;

static bool isEnemyLerping = false;
static float lerpStartTime = 0.0f;
static Vector3 enemyOriginPos;

float EnemyColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // สีแดง
float TeamColor[4]  = { 0.0f, 1.0f, 1.0f, 1.0f }; // สีฟ้า
float VehColor[4]   = { 0.0f, 1.0f, 0.0f, 1.0f }; // สีเขียว

std::string mono_to_utf8(monoString* monoStr) {
    if (!monoStr || (uintptr_t)monoStr <= 0x1000) return "";
    
    int len = monoStr->getLength(); 
    if (len <= 0 || len > 128) return "";

    std::string utf8;
    utf8.reserve(len * 3);

    for (int i = 0; i < len; i++) {
        uint16_t c = (uint16_t)get_Chars(monoStr, i); 
        if (c <= 0x7F) {
            utf8 += static_cast<char>(c);
        } else if (c <= 0x7FF) {
            utf8 += static_cast<char>(0xC0 | ((c >> 6) & 0x1F));
            utf8 += static_cast<char>(0x80 | (c & 0x3F));
        } else { 
            utf8 += static_cast<char>(0xE0 | ((c >> 12) & 0x0F));
            utf8 += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            utf8 += static_cast<char>(0x80 | (c & 0x3F));
        }
    }
    return utf8;
}

struct VehicleData {
    int id;
    float distance;
    Vector3 pos;
    const char* name;
};
std::vector<VehicleData> vehicleList;

void DrawESP(float screenWidth, float screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;
	
	ImColor currentEnemyColor = ImColor(EnemyColor[0], EnemyColor[1], EnemyColor[2], EnemyColor[3]);
	ImColor currentTeamColor  = ImColor(TeamColor[0], TeamColor[1], TeamColor[2], TeamColor[3]);
	ImColor currentVehColor   = ImColor(VehColor[0], VehColor[1], VehColor[2], VehColor[3]);

    if (LogSend) {
        char textBuf[256];
        if (is_data_received) {
            snprintf(textBuf, sizeof(textBuf),
                "Last Sent Packet Info:\n"
                "Message ID: %u (0x%X)\n"
                "Msg Pointer: %p\n"
                "Send Option: %d\n"
                "Cache Msg: %s",
                hooked_messageID, hooked_messageID, hooked_msg_ptr, hooked_sendOption, hooked_cacheMsgAnyWay ? "True" : "False");
        } else {
            snprintf(textBuf, sizeof(textBuf), "Waiting for Send() to be called...");
        }

        ImVec2 textSize = ImGui::CalcTextSize(textBuf);

        float paddingX = 14.0f;
        float paddingY = 8.0f;
        float rounding = 4.0f;

        float x = ImGui::GetIO().DisplaySize.x - textSize.x - paddingX - 25.0f;
        float y = (ImGui::GetIO().DisplaySize.y - (textSize.y + paddingY * 2.0f)) / 2.0f;

        ImVec2 bgMin = ImVec2(x - paddingX, y - paddingY);
        ImVec2 bgMax = ImVec2(x + textSize.x + paddingX, y + textSize.y + paddingY);

        draw->AddRectFilled(bgMin, bgMax, ImColor(15, 15, 15, 220), rounding);
        draw->AddRect(bgMin, bgMax, ImColor(255, 0, 0, 40), rounding, 0, 1.0f);

        ImColor accentColor(230, 30, 30, 255);
        draw->AddRectFilled(ImVec2(bgMin.x, bgMin.y), ImVec2(bgMin.x + 3.0f, bgMax.y), accentColor, rounding, ImDrawFlags_RoundCornersLeft);

        draw->AddText(ImVec2(x + 3.0f, y), ImColor(255, 255, 255, 255), textBuf);
    }
    if (!EnableEsp) return;

    void* current_Match = Curent_Match();
    void* local_player = GetLocalPlayer(current_Match);
    if (!current_Match || !local_player) return;

    void* camera = Camera_main();
    int inimigo_num = 0;
auto playerDict = *(monoDictionary<void*, void*>**)((uintptr_t)current_Match + ListPlayer);

if (playerDict && (uintptr_t)playerDict > 0x1000 && playerDict->entries) {
    int count = playerDict->getCapacity();
    
    for (int i = 0; i < count; i++) {
        auto& entry = playerDict->getEntry(i);
        if (entry.hashCode < 0) continue;

        void* playerObj = entry.value;
        if (!playerObj || (uintptr_t)playerObj <= 0x1000 || playerObj == local_player) continue;
        bool isTeammate = get_isLocalTeam(playerObj);
        // ==================== [ 1. ฝั่งเพื่อนร่วมทีม ] ====================
        if (isTeammate) {
            if (EnableTeamESP) {
                void* teamObj = playerObj; // กำหนดตัวแปรเดิมเพื่อไม่ให้โค้ดด้านล่างพัง
                
                Vector3 T_Pos = getPosition(teamObj);
                Vector3 T_HeadPos = T_Pos + Vector3(0, 1.7f, 0); 
                Vector3 T_Screen = WorldToScreenPoint(camera, T_Pos);
                Vector3 T_HeadScreen = WorldToScreenPoint(camera, T_HeadPos);

                if (T_HeadScreen.z > 1) {
                    float T_Hight = abs(T_HeadScreen.y - T_Screen.y) * 1.1f;
                    float T_Width = T_Hight * 0.45f;
                    float T_X = T_HeadScreen.x - T_Width / 2.f;
                    float T_Y = screenHeight - T_HeadScreen.y;

                    ImColor teamColor = currentTeamColor;
                    
                    draw->AddRect(ImVec2(T_X, T_Y), ImVec2(T_X + T_Width, T_Y + T_Hight), teamColor, 0.0f, 0, 1.0f);
                    draw->AddLine(ImVec2(screenWidth * 0.5f, 0.0f), ImVec2(T_X + T_Width / 2, T_Y + T_Hight), teamColor, 0.8f);


                    monoString* Nick = get_NickName(teamObj);
                    std::string nameStr = (Nick != nullptr) ? "Member" : "Teammate";
                    float dist = Vector3::Distance(getPosition(local_player), T_Pos);
                    
                    char infoText[64];
                    snprintf(infoText, sizeof(infoText), "%s | %.0fm", nameStr.c_str(), dist);

                    ImFont* font = ImGui::GetFont();
                    float fontSize = 18.0f;
                    ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, infoText);

                    float paddingX = 6.0f, paddingY = 3.0f;
                    float nameX = T_X + (T_Width / 2.0f) - (textSize.x / 2.0f);
                    float nameY = T_Y - textSize.y - 8.0f;
                    float barWidth = 4.0f, leftOffset = barWidth + 2.0f;

                    draw->AddRectFilled(ImVec2(nameX - paddingX + leftOffset, nameY - paddingY), ImVec2(nameX + textSize.x + paddingX + leftOffset, nameY + textSize.y + paddingY), ImColor(0, 0, 0, 110), 0.0f);
                    draw->AddRectFilled(ImVec2(nameX - paddingX + leftOffset, nameY - paddingY), ImVec2(nameX - paddingX + leftOffset + barWidth, nameY + textSize.y + paddingY), teamColor, 0.0f);
                    draw->AddText(font, fontSize, ImVec2(nameX + leftOffset + barWidth + 2.0f, nameY), teamColor, infoText);
                }
            }
        }
        // ==================== [ 2. ฝั่งศัตรู ] ====================
        else {
            void* closestEnemy = playerObj; // กำหนดตัวแปรเดิมเพื่อไม่ให้โค้ดด้านล่างพัง
            
            if (get_isVisible(closestEnemy)) {
                inimigo_num++;

                Vector3 Toepos = getPosition(closestEnemy);
                Vector3 Toeposi = WorldToScreenPoint(camera, Vector3(Toepos.x, Toepos.y, Toepos.z));
                if (Toeposi.z < 1) continue;

                // ลดการดึงตำแหน่งซ้ำ โดยใช้พิกัดเท้า (Toepos) มาบวกความสูงแทน getPosition อีกรอบ
                Vector3 HeadPos = Toepos + Vector3(0, 1.7f, 0);
                Vector3 HeadPosition = WorldToScreenPoint(camera, Vector3(HeadPos.x, HeadPos.y, HeadPos.z));
                if (HeadPosition.z < 1) continue;

                float distance = Vector3::Distance(getPosition(local_player), HeadPos);
                float Hight = abs(HeadPosition.y - Toeposi.y) * (1.2 / 1.1);
                float Width = Hight * 0.50f;
                Rect rect(HeadPosition.x - Width / 2.f, screenHeight - HeadPosition.y, Width, Hight);

                bool isKnocked = get_IsDieing(closestEnemy);
			   if (closestEnemy && EspGrenade) {
                        Vector3 LocalHead = GetHeadPosition(local_player);
                        Vector3 EnemyHead = GetHeadPosition(closestEnemy);

                        GrenadeLine_DrawLine(LineGrenade, LocalHead, LocalHead, Vector3(0, 0.1f, 0) * 0.1);
                        if (RenderLine) {
                            UnityColor RedColor = {1.0f, 0.0f, 0.0f, 1.0f};
                            LineRenderer_SetColor(RenderLine, RedColor);
                            LineRenderer_Set_PositionCount(RenderLine, 0x2);
                            LineRenderer_SetPosition(RenderLine, 0, LocalHead);
                            LineRenderer_SetPosition(RenderLine, 1, EnemyHead);
                        }
                    }
                void* imo = get_imo(local_player);
                if (imo) {
                       Vector3 LocalHead = GetAttackableCenterWS(local_player);
                       Vector3 EnemyHead = GetAttackableCenterWS(closestEnemy);
                       if (EspFire) set_esp1(imo, LocalHead, EnemyHead, closestEnemy);
                       if (EspFireBlue) set_esp3(imo, LocalHead, EnemyHead);
                }
                if (EspLine) {
                    ImVec2 startPoint = ImVec2(screenWidth * 0.5f, 0.0f);
                    ImVec2 hitPoint = ImVec2(rect.x + rect.w * 0.5f, rect.y);
                    ImColor lineColor = isKnocked ? ImColor(255, 0, 0, 255) : currentEnemyColor;
                    draw->AddLine(startPoint, hitPoint, lineColor, 0.8f);
                }
                    if (ESPArrow && !get_IsDieing(closestEnemy)) {
                        ImDrawList* fg2 = ImGui::GetForegroundDrawList();
                        float xx = rect.x + rect.w / 2.0f, yy = rect.y - 15.0f, arrowSize = 12.0f;
                        fg2->AddTriangleFilled(ImVec2(xx, yy), ImVec2(xx - arrowSize / 2.0f, yy + arrowSize), ImVec2(xx + arrowSize / 2.0f, yy + arrowSize), IM_COL32(255, 0, 0, 255));
                        fg2->AddTriangle(ImVec2(xx, yy), ImVec2(xx - arrowSize / 2.0f, yy + arrowSize), ImVec2(xx + arrowSize / 2.0f, yy + arrowSize), IM_COL32(255, 255, 255, 255));
                    }

                    if (EspBox4) {
                        ImColor boxColor = isKnocked ? ImColor(255, 0, 0, 255) : currentEnemyColor;
                        draw->AddRectFilled(ImVec2(rect.x, rect.y), ImVec2(rect.x + rect.w, rect.y + rect.h), isKnocked ? ImColor(255, 0, 0, 50) : ImColor(0, 0, 0, 80), 4.0f);
                        draw->AddRect(ImVec2(rect.x, rect.y), ImVec2(rect.x + rect.w, rect.y + rect.h), boxColor, 4.0f, 0, 1.2f);
                    }
                    if (EspBox3D) {
                        ImColor boxColor = isKnocked ? ImColor(255, 0, 0, 255) : currentEnemyColor;
                        float depth = rect.w * 0.25f;
                        ImVec2 f_min(rect.x, rect.y), f_max(rect.x + rect.w, rect.y + rect.h);
                        ImVec2 b_min(rect.x - depth, rect.y - depth), b_max(rect.x + rect.w - depth, rect.y + rect.h - depth);
                        draw->AddRect(f_min, f_max, boxColor, 0, 0, 1.0f); draw->AddRect(b_min, b_max, boxColor, 0, 0, 1.0f);
                        draw->AddLine(f_min, b_min, boxColor, 1.0f); draw->AddLine(ImVec2(f_max.x, f_min.y), ImVec2(b_max.x, b_min.y), boxColor, 1.0f);
                        draw->AddLine(ImVec2(f_min.x, f_max.y), ImVec2(b_min.x, b_max.y), boxColor, 1.0f); draw->AddLine(f_max, b_max, boxColor, 1.0f);
                    }
// 1. วาดหลอดเลือด (Health Bar ด้านซ้ายของกล่อง)
if (EspHealthBar && !isKnocked) {
    float barW = 3.0f;
    float barX = rect.x - barW - 4.0f;
    float barY = rect.y;
    float barH = rect.h;
    
    float maxHealth = get_MaxHP(closestEnemy);
    float currentHealth = GetHp(closestEnemy);
    float healthPct = (maxHealth > 0) ? currentHealth / maxHealth : 0.0f;
    if (healthPct > 1.0f) healthPct = 1.0f;

    // พื้นหลังหลอดเลือด
    draw->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barW, barY + barH), ImColor(0, 0, 0, 150));
    
    // สีหลอดเลือดเปลี่ยนตามเลือด (เขียว -> เหลือง -> แดง)
    ImColor hpColor = (healthPct > 0.5f) ? ImColor(0, 255, 0, 255) : (healthPct > 0.25f) ? ImColor(255, 255, 0, 255) : ImColor(255, 0, 0, 255);
    float currentBarH = barH * healthPct;
    draw->AddRectFilled(ImVec2(barX, barY + (barH - currentBarH)), ImVec2(barX + barW, barY + barH), hpColor);
}

// 2. วาดชื่อ (ด้านบนกล่อง)
if (EspName) {
    monoString* Nick = get_NickName(closestEnemy);
    std::string nameStr = (Nick != nullptr) ? mono_to_utf8(Nick) : "Unknown";
    bool isBot = *(bool*)((uintptr_t)closestEnemy + m_isClientBot);
    std::string displayName = (isBot ? "[BOT] " : "") + nameStr;

    ImFont* font = ImGui::GetFont();
    ImVec2 textSize = font->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, displayName.c_str());
    float nameX = rect.x + (rect.w / 2.0f) - (textSize.x / 2.0f);
    float nameY = rect.y - textSize.y - 4.0f;

    draw->AddText(font, 16.0f, ImVec2(nameX, nameY), ImColor(255, 255, 255, 255), displayName.c_str());
}

// 3. วาดระยะห่าง (ด้านล่างกล่อง)
if (EspDistance) {
    char distBuf[32];
    snprintf(distBuf, sizeof(distBuf), "%.0fm", distance);
    
    ImFont* font = ImGui::GetFont();
    ImVec2 textSize = font->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, distBuf);
    float distX = rect.x + (rect.w / 2.0f) - (textSize.x / 2.0f);
    float distY = rect.y + rect.h + 2.0f;

    draw->AddText(font, 16.0f, ImVec2(distX, distY), ImColor(200, 200, 200, 255), distBuf);
}
                if (PullEnemyV1) {
                    void* enTr = Component_GetTransform(closestEnemy);
                    void* localTr = Component_GetTransform(local_player);

                    if (enTr && localTr) {
                        Vector3 myPos = Transform_INTERNAL_GetPosition(localTr);
                        Vector3 enPos = Transform_INTERNAL_GetPosition(enTr);
                        
                        Vector3 forward = GetForward(localTr); 
                        Vector3 targetPos = myPos + (forward * 1.0f); 

                        if (Vector3::Distance(myPos, enPos) < 8.0f) {
                            float lerpX = Lerp(enPos.x, targetPos.x, 8.0f * ImGui::GetIO().DeltaTime);
                            float lerpY = Lerp(enPos.y, targetPos.y, 8.0f * ImGui::GetIO().DeltaTime);
                            float lerpZ = Lerp(enPos.z, targetPos.z, 8.0f * ImGui::GetIO().DeltaTime);
                            Transform_INTERNAL_SetPosition(enTr, Vvector3(lerpX, lerpY, lerpZ));
                        }
                    }
                }
                if (TPPlayer) {
                    void* trLocal = Component_GetTransform(local_player);
                    void* trEnemy = Component_GetTransform(closestEnemy);
                    if (trEnemy) {
                        Vector3 enemyPos = getPosition(trEnemy);
                        Vector3 backPos = enemyPos - (GetForward(trEnemy) * 1.0f);
                        Transform_INTERNAL_SetPosition(trLocal, Vvector3(backPos.x, backPos.y, backPos.z));
                        TPPlayer = false;
                    }
                }
if (Underkill) {
    void* trLocal = Component_GetTransform(local_player);
    void* trEnemy = Component_GetTransform(closestEnemy);
    if (trEnemy) {
        if (!isInitialized || closestEnemy != lastEnemy) {
            initEnemyPos = getPosition(trEnemy);
            lastEnemy = closestEnemy;
            isInitialized = true;
        }
        Transform_INTERNAL_SetPosition(trEnemy, Vvector3(initEnemyPos.x, initEnemyPos.y - 3.0f, initEnemyPos.z));
        Transform_INTERNAL_SetPosition(trLocal, Vvector3(initEnemyPos.x, initEnemyPos.y - 1.0f, initEnemyPos.z));
    }
} else {
    isInitialized = false;
    lastEnemy = nullptr;
}
if (Telekill) {
    void* trLocal = Component_GetTransform(local_player);
    void* trEnemy = Component_GetTransform(closestEnemy);
    if (trEnemy) {
        Vector3 myPos = Transform_INTERNAL_GetPosition(trLocal);
        Vector3 enemyPos = getPosition(trEnemy);
        Vector3 backPos = enemyPos - (GetForward(trEnemy) * 1.0f);
        float dx = backPos.x - myPos.x;
        float dy = backPos.y - myPos.y;
        float dz = backPos.z - myPos.z;
        float distance = sqrt(dx * dx + dy * dy + dz * dz);

        if (distance > 3.0f) {
             float ratio = 3.0f / distance;
            float nextX = myPos.x + (dx * ratio);
            float nextY = myPos.y + (dy * ratio);
            float nextZ = myPos.z + (dz * ratio);
            Transform_INTERNAL_SetPosition(trLocal, Vvector3(nextX, nextY, nextZ));
        } else {
            Transform_INTERNAL_SetPosition(trLocal, Vvector3(backPos.x, backPos.y, backPos.z));
        }
    }
}
if (TelekillFW) {
    void* trLocal = Component_GetTransform(local_player);
    void* trEnemy = Component_GetTransform(closestEnemy);
    if (trEnemy) {
        Vector3 myPos = Transform_INTERNAL_GetPosition(trLocal);
        Vector3 enemyPos = getPosition(trEnemy);
        Vector3 backPos = enemyPos - (GetForward(trEnemy) * 1.0f);
        float dx = backPos.x - myPos.x;
        float dy = backPos.y - myPos.y;
        float dz = backPos.z - myPos.z;
        float distance = sqrt(dx * dx + dy * dy + dz * dz);

        if (distance > 3.0f) {
             float ratio = 3.0f / distance;
            float nextX = myPos.x + (dx * ratio);
            float nextY = myPos.y + (dy * ratio);
            float nextZ = myPos.z + (dz * ratio);
            Transform_INTERNAL_SetPosition(trLocal, Vvector3(nextX, nextY, nextZ));
        } else {
            Transform_INTERNAL_SetPosition(trLocal, Vvector3(backPos.x, backPos.y, backPos.z));
        }
    }
}
                if (EnemyLerp) {
                    void* enTr = Component_GetTransform(closestEnemy);
                    if (enTr) {
                        Vector3 currentPos = Transform_INTERNAL_GetPosition(enTr);
                        if (!isEnemyLerping) { lerpStartTime = ImGui::GetTime(); isEnemyLerping = true; }
                        float elapsed = (float)(ImGui::GetTime() - lerpStartTime);
                        float progress = (fmod(elapsed, 1.0f) > 0.5f) ? 1.0f : 0.0f;
                        float offsetX = (progress == 0.0f) ? -8.0f : 5.0f;
                        float offsetZ = (progress == 0.0f) ? -8.0f : 5.0f;
                        float lerpedX = Lerp(currentPos.x, currentPos.x + offsetX, 8.0f * ImGui::GetIO().DeltaTime);
                        float lerpedZ = Lerp(currentPos.z, currentPos.z + offsetZ, 8.0f * ImGui::GetIO().DeltaTime);
                        float finalY = currentPos.y;
                        if (EnableYLerp) {
                            float offsetY = (progress == 0.0f) ? -2.0f : 2.0f;
                            finalY = Lerp(currentPos.y, currentPos.y + offsetY, 8.0f * ImGui::GetIO().DeltaTime);
                        }
                        Transform_INTERNAL_SetPosition(enTr, Vvector3(lerpedX, finalY, lerpedZ));
                    }
                } else { isEnemyLerping = false; }
                if (EspBox2) draw->AddRect(ImVec2(rect.x, rect.y), ImVec2(rect.x + rect.w, rect.y + rect.h), isKnocked ? ImColor(255, 0, 0, 255) : ImColor(255, 255, 255, 255), 0.0f, 0, 0.1f);
            }
        }
    }
}
    if (ShowFOV) draw->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), Fov_Aim * (screenWidth / 180.0f), ImColor(220, 20, 60, 255), 100, 1.0f);
	vehicleList.clear(); 
	static uintptr_t offset_m_Vehicle = 0;
	if (offset_m_Vehicle == 0) {
    	offset_m_Vehicle = Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("LevelVehicle"), OBFUSCATE("m_Vehicle"));
	}
	auto vehicles = *(monoDictionary<void*, void*>**)((uintptr_t)current_Match + ListVehicle);
	std::vector<void*> targetCars;
	Vector3 closestVehiclePos = {0, 0, 0};
	float closestDist = 999999.0f;
	bool foundAnyVehicle = false;

	if (vehicles && (uintptr_t)vehicles > 0x1000 && vehicles->entries) {
    	int count1 = vehicles->getCapacity();
    	for (int i = 0; i < count1; i++) {
        	auto& entry1 = vehicles->getEntry(i);
        	if (entry1.hashCode < 0) continue;

        	void* levelVehicle = entry1.value;
        	if (!levelVehicle || (uintptr_t)levelVehicle < 0x1000) continue;

        	Vector3 vWorldPos = Transform_INTERNAL_GetPosition(Component_GetTransform(levelVehicle));
        	float vDistance = Vector3::Distance(getPosition(local_player), vWorldPos);
        	if (vDistance < closestDist) { 
            	closestDist = vDistance; 
            	closestVehiclePos = vWorldPos; 
            	foundAnyVehicle = true; 
        	}
		    if (teleportToCar && foundAnyVehicle){
				void* trLocal = Component_GetTransform(local_player);
				Transform_INTERNAL_SetPosition(trLocal, Vvector3(closestVehiclePos.x, closestVehiclePos.y + 1.5f, closestVehiclePos.z));
    		}
        	if (EspVehicle) {
            	Vector3 vScreenPos = WorldToScreenPoint(camera, vWorldPos);
            	if (vScreenPos.z > 0) {
	                ImVec2 vDrawPos = ImVec2(vScreenPos.x, screenHeight - vScreenPos.y);
	                draw->AddLine(ImVec2(screenWidth / 2, screenHeight), vDrawPos, currentVehColor, 1.5f);
	                char vBuf[64]; snprintf(vBuf, sizeof(vBuf), "ID: %d | [%.0fm]", i, vDistance);
	                draw->AddText(ImGui::GetFont(), 16.0f, vDrawPos, currentVehColor, vBuf);
	            }
	        }
	    }
	}
}
