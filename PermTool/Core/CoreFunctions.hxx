#pragma once

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

void Core_ImGui_ResourceHandleSelectable(const char* p_Name, uint32_t p_NameUID, uint32_t p_OwnerUID)
{
    if (!p_NameUID)
        return;

    ImGui::Text("%s:", p_Name);
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Text, IMGUI_COLOR_TEXT2);

    CResourceData* m_HandleResourceData = Core_FindResourceByName(p_NameUID);
    if (m_HandleResourceData)
    {
        std::string m_ResourceName = m_HandleResourceData->GetName();
        if (ImGui::Selectable(Format::Get("%s##%u", m_ResourceName.c_str(), p_OwnerUID), false))
            Core_SelectResourceName(p_NameUID);

        ImGui::PopStyleColor();

        ImGui::SetItemTooltip(m_ResourceName.c_str());
        return;
    }
    else
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