#pragma once

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
        if (ImGui::Selectable(Format::Get("%s##%u", m_HandleResourceData->m_DebugName, p_OwnerUID), false))
            Core_SelectResourceName(p_NameUID);

        ImGui::PopStyleColor();

        ImGui::SetItemTooltip(m_HandleResourceData->m_DebugName);
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