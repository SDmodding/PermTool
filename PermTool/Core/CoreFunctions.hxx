#pragma once

void Core_ImGui_ColorInfo(std::string p_Label, float* p_FloatColor)
{
    ImGui::ColorButton(Format::Get("##%s_Color", p_Label.c_str()), ImVec4(p_FloatColor[0], p_FloatColor[1], p_FloatColor[2], p_FloatColor[3]));

    ImGui::SameLine();

    uint8_t m_Color[4];
    Core_ConvertFloat4Color2UInt(p_FloatColor, m_Color);

    char m_Buffer[64];
    sprintf_s(m_Buffer, sizeof(m_Buffer), "rgba(%d,%d,%d,%d); #%02X%02X%02X%02X", m_Color[0], m_Color[1], m_Color[2], m_Color[3], m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

    ImGui::SetNextItemWidth(-50.f);
    ImGui::InputText(Format::Get("%s##Input", p_Label.c_str()), m_Buffer, sizeof(m_Buffer), ImGuiInputTextFlags_ReadOnly);
}

bool Core_ImGui_InputUInt(const char* p_Label, uint32_t* p_Value, uint32_t p_Step, uint32_t p_StepFast , ImGuiInputTextFlags p_Flags)
{
    const char* m_Format = (p_Flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%u";
    return ImGui::InputScalar(p_Label, ImGuiDataType_U32, (void*)p_Value, (void*)(p_Step > 0 ? &p_Step : 0), (void*)(p_StepFast > 0 ? &p_StepFast : 0), m_Format, p_Flags);
}

void Core_ImGui_TextSuffix(const char* p_Prefix, const char* p_Suffix, float p_StartX)
{
    ImGui::Text("%s:", p_Prefix);
    ImGui::SameLine(p_StartX);
    ImGui::PushStyleColor(ImGuiCol_Text, IMGUI_COLOR_TEXT2);
    ImGui::Text(p_Suffix);
    ImGui::PopStyleColor();
}

bool Core_ImGui_RightClickItemPopup(const char* p_StrID)
{
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
        if (p_StrID)
            ImGui::OpenPopup(p_StrID);
        return true;
    }
    return false;
}

bool Core_ImGui_RightClickItemPopupNoHover(const char* p_StrID)
{
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        if (p_StrID)
            ImGui::OpenPopup(p_StrID);
        return true;
    }
    return false;
}

bool Core_ImGui_ToolTipHover()
{
    return (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) && ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, ImGuiWindowFlags_None));
}

void Core_ImGui_ResourceHandleSelectable(const char* p_Name, uint32_t p_NameUID, float p_LineOffset, const char* p_ResourceName)
{
    if (!p_NameUID) {
        return;
    }

    ImGui::Text("%s:", p_Name);
    ImGui::SameLine(p_LineOffset);

    ImGui::PushStyleColor(ImGuiCol_Text, IMGUI_COLOR_TEXT2);

    CResourceData* m_HandleResourceData = Core_FindResourceByName(p_NameUID);
    if (m_HandleResourceData)
    {
        std::string m_ResourceName = m_HandleResourceData->GetName();
        if (ImGui::Selectable(Format::Get("%s##%u", m_ResourceName.c_str()), false))
            Core_SelectResourceName(p_NameUID);

        ImGui::PopStyleColor();

        ImGui::SetItemTooltip(m_ResourceName.c_str());
        return;
    }
    
    if (p_ResourceName) 
    {
        ImGui::Text(p_ResourceName);
        ImGui::PopStyleColor();

        ImGui::SetItemTooltip(p_ResourceName);
        return;
    }
    
    ImGui::Text("0x%X", p_NameUID);
    ImGui::PopStyleColor();
}

void Core_SelectResourceName(uint32_t p_NameUID) { g_Core.m_SelectResourceNameUID = p_NameUID; }

CResourceData* Core_FindResourceByName(uint32_t p_NameUID)
{
    for (auto& m_PermPtr : g_Core.m_Perms)
    {
        CPerm* m_Perm = m_PermPtr.get();
        CResourceData* m_ResourceData = m_Perm->GetResourceData();
        if (m_ResourceData && m_ResourceData->m_NameUID == p_NameUID)
            return m_ResourceData;
    }

    return nullptr;
}

void Core_ConvertFloat4Color2UInt(float* p_Floats, uint8_t* p_UInt)
{
    for (int i = 0; 4 > i; ++i)
        p_UInt[i] = static_cast<uint8_t>(fmaxf(0.f, fminf(p_Floats[i] * 255.f, 255.f)));
}