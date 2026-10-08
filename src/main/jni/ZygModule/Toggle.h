#pragma once
#include "imgui_internal.h"
bool ToggleSYN(const char* label, bool* v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImGuiID id = window->GetID(label);

    const float height = 28.0f;
    const float width  = 60.0f;
    const float radius = height * 0.48f;

    const float circleSizeOn  = radius - 2.5f;
    const float circleSizeOff = radius - 4.0f;

    ImVec2 label_size = ImGui::CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;

    float avail_width = ImGui::GetContentRegionAvail().x;
    ImRect total_bb(pos, ImVec2(pos.x + avail_width, pos.y + height));
    ImGui::ItemSize(total_bb, style.FramePadding.y);

    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);

    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    // Animation
    static std::map<ImGuiID, float> anim;
    float& t = anim[id];
    float target = *v ? 1.0f : 0.0f;
    t = ImLerp(t, target, g.IO.DeltaTime * 12.0f);

    ImDrawList* draw = window->DrawList;
    float centerY = total_bb.Min.y + (total_bb.GetHeight() * 0.5f);

    ImVec2 text_pos(total_bb.Min.x, centerY - label_size.y * 0.5f);
    
    float rightEdge = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    float toggle_x = rightEdge - width - 10.0f;
    ImVec2 toggle_pos(toggle_x, centerY - height * 0.5f);

    // กำหนดสี (ใช้ col_bg, col_border, col_circle ตามระบบ)
    ImU32 col_bg     = *v ? IM_COL32(55, 55, 55, 255)   : IM_COL32(30, 30, 30, 255);
    ImU32 col_border = *v ? IM_COL32(90, 90, 90, 255)   : IM_COL32(50, 50, 50, 255);
    ImU32 col_circle = *v ? IM_COL32(200, 200, 200, 255) : IM_COL32(140, 140, 140, 255);

    // 1. วาดพื้นหลัง
    draw->AddRectFilled(toggle_pos, ImVec2(toggle_pos.x + width, toggle_pos.y + height), col_bg, radius);

    // 2. วาดเส้นขอบ
    draw->AddRect(toggle_pos, ImVec2(toggle_pos.x + width, toggle_pos.y + height), col_border, radius, 0.0f, 1.5f);

    // 3. คำนวณตำแหน่งวงกลมและอนิเมชันขนาด
    float base_x = toggle_pos.x + radius + (width - radius * 2.0f) * t;
    float offset = (*v) ? 0.0f : -2.0f;
    float circle_x = base_x + offset;
    float circleSize = ImLerp(circleSizeOff, circleSizeOn, t);

    // วาดวงกลมตัวเลื่อน
    draw->AddCircleFilled(ImVec2(circle_x, toggle_pos.y + radius), circleSize, col_circle);

    // วาดข้อความฝั่งซ้าย
    draw->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

    return pressed;
}


bool ToggleSYN1(const char* label, bool* v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImGuiID id = window->GetID(label);

    const float height = 24.0f;           
    const float width  = height * 2.0f;  
    const float radius = height * 0.50f;

    const float circleSizeOn  = radius - 3.5f;
    const float circleSizeOff = radius - 5.0f;

    ImVec2 label_size = ImGui::CalcTextSize(label);  
    ImVec2 pos = window->DC.CursorPos;  

    float avail_width = ImGui::GetContentRegionAvail().x;  
    ImRect total_bb(pos, ImVec2(pos.x + avail_width, pos.y + height));  
    ImGui::ItemSize(total_bb, style.FramePadding.y);  

    if (!ImGui::ItemAdd(total_bb, id))  
        return false;   

    // --- คงลอจิกเดิม: กดค้าง (Hold to Toggle) ---
    bool hovered, held;
    ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    
    if (held) 
        *v = true;   // กดค้างไว้เป็น true
    else 
        *v = false;  // ปล่อยแล้วเป็น false

    // Animation Smooth ลื่นๆ
    static std::map<ImGuiID, float> anim;
    float& t = anim[id];
    float target = *v ? 1.0f : 0.0f;
    t = ImLerp(t, target, g.IO.DeltaTime * 14.0f);

    ImDrawList* draw = window->DrawList;
    float centerY = total_bb.Min.y + (total_bb.GetHeight() * 0.5f);

    ImVec2 text_pos(total_bb.Min.x, centerY - label_size.y * 0.5f);  

    // คำนวณตำแหน่ง Toggle ชิดขวาแบบ FIX พร้อม Manual Offset 
    float rightEdge = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    float toggle_x = rightEdge - width - 10.0f;
    ImVec2 toggle_pos(toggle_x, centerY - height * 0.5f + 1.0f);

    // กำหนดสีคุมโทน
    ImU32 col_bg     = *v ? IM_COL32(75, 75, 75, 255)   : IM_COL32(40, 40, 40, 255);
    ImU32 col_border = *v ? IM_COL32(115, 115, 115, 255) : IM_COL32(70, 70, 70, 255);
    ImU32 col_circle = *v ? IM_COL32(180, 180, 180, 255) : IM_COL32(110, 110, 110, 255);

    // 1. วาดพื้นหลัง
    draw->AddRectFilled(toggle_pos, ImVec2(toggle_pos.x + width, toggle_pos.y + height), col_bg, radius);

    // 2. วาดเส้นขอบ
    draw->AddRect(toggle_pos, ImVec2(toggle_pos.x + width, toggle_pos.y + height), col_border, radius, 0.0f, 1.5f);

    // 3. คำนวณตำแหน่งและขนาดวงกลมตัวเลื่อน
    float base_x = toggle_pos.x + radius + (width - radius * 2.0f) * t;
    float circle_x = base_x + ((*v) ? 0.0f : -1.0f);
    float circleSize = ImLerp(circleSizeOff, circleSizeOn, t);

    draw->AddCircleFilled(ImVec2(circle_x, toggle_pos.y + radius), circleSize, col_circle);

    // 4. วาดข้อความฝั่งซ้าย
    draw->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

    return held;
}

void ToggleSYNF(const char* str_id, bool* v)
{
    float height = 40.5f;           
    float width  = height * 2.0f;  
    float radius = height * 0.50f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // ===== Text (ซ้าย) =====
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(str_id);
    ImGui::SameLine();

    // ===== Toggle (ขวาแบบ FIX) =====
    float rightEdge = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

    ImGui::SetCursorScreenPos(
        ImVec2(rightEdge - width - 10.0f, ImGui::GetCursorScreenPos().y + 1.5f)
    );

    ImVec2 p = ImGui::GetCursorScreenPos();

    // Click Logic
    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked()) *v = !*v;

    // Animation Logic
    float t = *v ? 1.0f : 0.0f;
    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id)) {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? t_anim : (1.0f - t_anim);
    }

    // กำหนดสี
    ImU32 col_bg     = *v ? IM_COL32(75, 75, 75, 255)   : IM_COL32(40, 40, 40, 255);
    ImU32 col_border = *v ? IM_COL32(115, 115, 115, 255) : IM_COL32(70, 70, 70, 255);
    ImU32 col_circle = *v ? IM_COL32(180, 180, 180, 255) : IM_COL32(110, 110, 110, 255);

    // 1. วาดพื้นหลัง
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, radius);

    // 2. วาดเส้นขอบ
    draw_list->AddRect(p, ImVec2(p.x + width, p.y + height), col_border, radius, 0, 1.5f);

    // 3. วงกลมตัวเลื่อน (ปรับระยะขอบตามขนาดใหญ่)
    float circle_radius = radius - 4.0f;
    draw_list->AddCircleFilled(
        ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius),
        circle_radius, 
        col_circle
    );
}


void ToggleSYN1F(const char* str_id, bool* v)
{
    float height = 40.5f;           
    float width  = height * 2.0f;  
    float radius = height * 0.50f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // ===== Text (ซ้าย) =====
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(str_id);
    ImGui::SameLine();

    // ===== Toggle (ขวาแบบ FIX + Manual Offset) =====
    float rightEdge = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

    ImGui::SetCursorScreenPos(
        ImVec2(rightEdge - width - 10.0f, ImGui::GetCursorScreenPos().y + 1.5f)
    );

    ImVec2 p = ImGui::GetCursorScreenPos();

    // --- ลอจิกกดค้าง (Hold to Toggle) ---
    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    
    if (ImGui::IsItemActive()) 
        *v = true;   
    else 
        *v = false;  

    // Animation Logic
    float t = *v ? 1.0f : 0.0f;
    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? t_anim : (1.0f - t_anim);
    }

    // กำหนดสี
    ImU32 col_bg     = *v ? IM_COL32(75, 75, 75, 255)   : IM_COL32(40, 40, 40, 255);
    ImU32 col_border = *v ? IM_COL32(115, 115, 115, 255) : IM_COL32(70, 70, 70, 255);
    ImU32 col_circle = *v ? IM_COL32(180, 180, 180, 255) : IM_COL32(110, 110, 110, 255);

    // 1. วาดพื้นหลัง
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, radius);

    // 2. วาดเส้นขอบ
    draw_list->AddRect(p, ImVec2(p.x + width, p.y + height), col_border, radius, 0, 1.5f);

    // 3. วงกลมตัวเลื่อน (ปรับระยะขอบตามขนาดใหญ่)
    float circle_radius = radius - 4.0f;
    draw_list->AddCircleFilled(
        ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius),
        circle_radius, 
        col_circle
    );
}


void ButtonSYN(const char* str_id, bool* v)
{
    float height = 50.0f; // ปรับลงเหลือ 50 เพื่อให้ดูคลีน ไม่เทอะทะเกินไป
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // 1. คำนวณความกว้าง (ลบออก 10 เพื่อให้มีช่องไฟชิดขวา)
    float startX = ImGui::GetCursorScreenPos().x;
    float endX   = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - 10.0f;
    float width  = endX - startX;

    ImVec2 p = ImGui::GetCursorScreenPos();

    // 2. InvisibleButton สำหรับดักคลิก
    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    // 3. สี: ON เทาขุ่น / OFF เทาเข้ม (คุมโทนดุ)
    ImU32 col_bg = *v 
        ? IM_COL32(100, 100, 100, 255)   // ON: เทาขุ่น
        : IM_COL32(40, 40, 40, 255);    // OFF: ดำเทา (ดิบกว่า 60,60,60)

    // 4. วาดพื้นหลัง (แก้เป็น 0.0f เพื่อความเหลี่ยมดุ)
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, 0.0f);

    // 5. วาดขอบขาว (หนา 1.5f คมๆ)
    draw_list->AddRect(p, ImVec2(p.x + width, p.y + height), IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.5f);

    // 6. วาด Text ตรงกลาง
    char buf[256];
    sprintf(buf, "%s [%s]", str_id, *v ? "ON" : "OFF");

    ImVec2 ts = ImGui::CalcTextSize(buf);
    draw_list->AddText(
        ImVec2(p.x + (width - ts.x) * 0.5f, p.y + (height - ts.y) * 0.5f),
        IM_COL32(255, 255, 255, 255), 
        buf
    );

    ImGui::Dummy(ImVec2(0.0f, 5.0f)); 
}


void ButtonSYN1(const char* str_id, bool* v)
{
    float height = 30.0f;
    float width  = height * 1.55f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // ===== Text (ซ้าย) =====
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(str_id);

    // ===== Button (ขวาแบบ FIX) =====
    ImGui::SameLine();

    float rightEdge =
        ImGui::GetWindowPos().x +
        ImGui::GetWindowContentRegionMax().x;

    ImGui::SetCursorScreenPos(
        ImVec2(rightEdge - width, ImGui::GetCursorScreenPos().y)
    );

    ImVec2 p = ImGui::GetCursorScreenPos();

    // --- หลักการทำงาน: กดค้างเป็น true ปล่อยเป็น false ---
    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    
    if (ImGui::IsItemActive()) 
        *v = true;   
    else 
        *v = false;  

    // ===== ระบบ Animation (ใช้โครงสร้างเดิมของคุณ) =====
    float t = *v ? 1.0f : 0.0f;
    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? t_anim : (1.0f - t_anim);
    }

    // สีเทาขุ่นตอนกด (Active) / สีเทาเข้มตอนปกติ (Inactive)
    ImU32 col_bg = *v
        ? IM_COL32(100, 100, 100, 255) 
        : IM_COL32(60, 60, 60, 255);   

    // วาดพื้นหลังปุ่ม
    draw_list->AddRectFilled(
        p,
        ImVec2(p.x + width, p.y + height),
        col_bg,
        height * 0.5f // ความโค้งมนเท่าเดิม
    );

    // ===== เปลี่ยนจากวงกลม เป็นคำว่า CLICK กลางปุ่ม =====
    const char* text = "CLICK";
    ImVec2 textSize = ImGui::CalcTextSize(text);
    
    // คำนวณตำแหน่งให้ Text อยู่กึ่งกลางปุ่ม
    ImVec2 textPos = ImVec2(
        p.x + (width - textSize.x) * 0.5f,
        p.y + (height - textSize.y) * 0.5f
    );

    // ใส่ Animation ความโปร่งใสของตัวหนังสือตามค่า t (Optional: ถ้าอยากให้ตัวหนังสือวูบวาบตามแรงกด)
    draw_list->AddText(textPos, IM_COL32(255, 255, 255, (int)(200 + (t * 55))), text);
}

inline bool BSSliderFloat(const char* label, float* value, float min, float max, const char* format = "%.2f") {
    const char* display_end = strstr(label, "##");
    int display_len = display_end ? (int)(display_end - label) : (int)strlen(label);

    if (display_len > 0) {
        ImGui::Text("%.*s", display_len, label);
        ImGui::SameLine();
    }

    ImGui::Text(format, *value);

    float sliderWidth = ImGui::GetContentRegionAvail().x;  
    ImVec2 pos = ImGui::GetCursorScreenPos();  
    ImDrawList* draw = ImGui::GetWindowDrawList();  
    
    // เพิ่มความสูงเป็น 20.0f เพื่อขยายพื้นที่กด (InvisibleButton) ให้ลากง่ายขึ้น
    float height = 20.0f;
    float trackRounding = 10.0f; // โค้งมนทรงแคปซูล (Pill shape)
    
    ImGui::InvisibleButton(label, ImVec2(sliderWidth, height)); 
    bool changed = false;  
    if (ImGui::IsItemActive()) {  
        float t = (ImGui::GetIO().MousePos.x - pos.x) / sliderWidth;  
        t = ImClamp(t, 0.0f, 1.0f);  
        *value = min + (max - min) * t;  
        changed = true;  
    }  
    
    float fraction = (*value - min) / (max - min);  
    
    // คำนวณตำแหน่ง Knob โดยเว้นระยะ Padding เพื่อไม่ให้ปุ่มหลุดขอบโค้ง
    float knobPadding = 10.0f;
    float knobX = pos.x + knobPadding + ((sliderWidth - (knobPadding * 2.0f)) * fraction); 

    ImU32 bgColor = ImColor(20, 20, 20, 255);       // ดำสนิท
    ImU32 activeColor = ImColor(180, 0, 0, 255);    // แดง BloodShot
    ImU32 knobColor = ImGui::IsItemActive() ? ImColor(255, 255, 255, 255) : ImColor(200, 200, 200, 255);

    // 1. แถบพื้นหลัง (ขอบมน)
    draw->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + sliderWidth, pos.y + height), bgColor, trackRounding);
    
    // 2. แถบสถานะสีแดง (ขอบมน)
    if (fraction > 0.0f) {
        float activeWidth = ImMax(knobX, pos.x + trackRounding);
        draw->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(activeWidth, pos.y + height), activeColor, trackRounding);
    }
    
    // 3. ปุ่มเลื่อน (Knob) ปรับเป็นสี่เหลี่ยมขอบมนหนา 12px นูนลอยขอบเล็กน้อยเพื่อให้แตะจับง่าย
    draw->AddRectFilled(ImVec2(knobX - 6.0f, pos.y - 1.0f), ImVec2(knobX + 6.0f, pos.y + height + 1.0f), knobColor, 4.0f);
    
    return changed;
}

//BS4484 YOU CAN DELETE CREDIT I DONT CARE
