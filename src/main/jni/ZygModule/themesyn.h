void ApplyDarkThemeBase(ImVec4 c_main, ImVec4 c_hover, ImVec4 c_active) {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                 = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]             = ImVec4(0.02f, 0.02f, 0.02f, 0.55f);
    colors[ImGuiCol_ChildBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.75f);
    colors[ImGuiCol_PopupBg]              = ImVec4(0.02f, 0.02f, 0.02f, 0.75f);
    colors[ImGuiCol_FrameBg]              = ImVec4(0.08f, 0.08f, 0.08f, 0.85f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBg]              = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    colors[ImGuiCol_MenuBarBg]            = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    colors[ImGuiCol_TabUnfocused]         = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_Button]               = ImVec4(0.15f, 0.15f, 0.15f, 0.60f); 
    colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_Border]               = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); 
    colors[ImGuiCol_ScrollbarGrab]        = c_main;
    colors[ImGuiCol_ScrollbarGrabHovered] = c_hover;
    colors[ImGuiCol_ScrollbarGrabActive]  = c_active;
    colors[ImGuiCol_CheckMark]            = c_main;
    colors[ImGuiCol_SliderGrab]           = c_main;
    colors[ImGuiCol_SliderGrabActive]     = c_active;
    colors[ImGuiCol_ButtonHovered]        = c_hover;
    colors[ImGuiCol_ButtonActive]         = c_active;
    colors[ImGuiCol_Header]               = c_main;
    colors[ImGuiCol_HeaderHovered]        = c_hover;
    colors[ImGuiCol_HeaderActive]         = c_active;
    colors[ImGuiCol_Separator]            = ImVec4(c_main.x, c_main.y, c_main.z, 0.25f);
    colors[ImGuiCol_SeparatorHovered]     = ImVec4(c_hover.x, c_hover.y, c_hover.z, 0.45f);
    colors[ImGuiCol_SeparatorActive]      = ImVec4(c_active.x, c_active.y, c_active.z, 0.65f);
    colors[ImGuiCol_ResizeGrip]           = ImVec4(c_main.x, c_main.y, c_main.z, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]    = ImVec4(c_hover.x, c_hover.y, c_hover.z, 0.67f);
    colors[ImGuiCol_ResizeGripActive]     = c_active;
    colors[ImGuiCol_Tab]                  = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TabHovered]           = c_hover;
    colors[ImGuiCol_TabActive]            = c_active;
    colors[ImGuiCol_PlotLines]            = c_main;
    colors[ImGuiCol_PlotLinesHovered]     = c_hover;
    colors[ImGuiCol_PlotHistogram]        = c_main;
    colors[ImGuiCol_PlotHistogramHovered] = c_hover;
    colors[ImGuiCol_TextSelectedBg]       = ImVec4(c_main.x, c_main.y, c_main.z, 0.30f);
    colors[ImGuiCol_DragDropTarget]       = c_hover;
    colors[ImGuiCol_NavHighlight]         = c_main;

    // ===== Layout & Styles =====
    style.WindowRounding    = 0.0f;
    style.ChildRounding     = 0.0f;
    style.WindowBorderSize  = 1.0f; 
    style.ChildBorderSize   = 0.5f; 
    style.PopupBorderSize   = 1.0f;
    style.FrameBorderSize   = 1.0f; 
    style.TabBorderSize     = 1.0f;
    style.ScrollbarSize     = 16.0f;      
    style.ScrollbarRounding = 3.0f; 
    style.GrabRounding      = 0.0f;
    style.TabRounding       = 0.0f;
    style.WindowPadding     = ImVec2(12, 12); 
    style.FramePadding      = ImVec2(6, 4);   
    style.ItemSpacing       = ImVec2(10, 8);  
    style.GrabMinSize       = 15.0f; 
}

// โค้ดสำหรับเรียกใช้แต่ละตีม (แค่เปลี่ยนสี)
void ApplyGreenTheme() {
    ApplyDarkThemeBase(
        ImVec4(0.00f, 0.65f, 0.00f, 1.0f), 
        ImVec4(0.00f, 0.65f, 0.00f, 1.0f), 
        ImVec4(0.00f, 0.65f, 0.00f, 1.0f)  
    );
}

void ApplyGreyTheme() {
    ApplyDarkThemeBase(
        ImVec4(0.35f, 0.35f, 0.35f, 1.0f),
        ImVec4(0.45f, 0.45f, 0.45f, 1.0f),
        ImVec4(0.55f, 0.55f, 0.55f, 1.0f)
    );
}

void ApplyRedTheme() {
    ApplyDarkThemeBase(
        ImVec4(0.65f, 0.00f, 0.00f, 1.0f),
        ImVec4(0.65f, 0.00f, 0.00f, 1.0f),
        ImVec4(0.65f, 0.00f, 0.00f, 1.0f)
    );
}

void ApplyBlueTheme() {
    ApplyDarkThemeBase(
        ImVec4(0.00f, 0.00f, 0.65f, 1.0f),
        ImVec4(0.00f, 0.00f, 0.65f, 1.0f),
        ImVec4(0.00f, 0.00f, 0.65f, 1.0f)
    );
}

