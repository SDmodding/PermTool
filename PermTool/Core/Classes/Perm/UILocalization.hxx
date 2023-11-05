#pragma once
#include "Dictionary/UILocalization.hxx"

class CUILocalization : public CPerm
{
public:
	std::vector<std::pair<uint32_t, const char*>> m_List;
    std::vector<std::pair<uint32_t, const char*>> m_SearchList;

	CUILocalization()
	{
        Dictionary::InitializeUILocalizationSymbols();
	}

    static __inline const char* GetKeyName(uint32_t p_Key)
    {
        const char* m_Key = Dictionary::g_UILocalizationSymbols.Get(p_Key);
        if (!m_Key)
            m_Key = Format::Get("0x%08X", p_Key);

        return m_Key;
    }

    static bool CompareKey(std::pair<uint32_t, const char*>& p_PairA, std::pair<uint32_t, const char*>& p_PairB)
    {
        const char* m_KeyA = Dictionary::g_UILocalizationSymbols.Get(p_PairA.first);
        const char* m_KeyB = Dictionary::g_UILocalizationSymbols.Get(p_PairB.first);

        // Check if one/both key names are invalid and we got only hash value...
        {
            if (m_KeyA == nullptr && m_KeyB == nullptr)
                return (p_PairB.first > p_PairA.first);

            if (m_KeyA == nullptr)
                return false;

            if (m_KeyB == nullptr)
                return true;
        }

        return (std::string(m_KeyB) > std::string(m_KeyA));
    }

    void OnDataLoad()
    {
        UFG::UILocalization_t* m_UILocalization = reinterpret_cast<UFG::UILocalization_t*>(GetResourceData());
        if (!m_UILocalization)
            return;

        char* m_StringPtr = m_UILocalization->GetStringPtr();
        if (!m_StringPtr)
            return;

        uint32_t* m_Symbols = m_UILocalization->GetSymbolsArray();
        if (!m_Symbols)
            return;

        uint32_t m_Index = 0;
        uint32_t m_StringOffset = 0;
        while (m_UILocalization->m_StringsSize > m_StringOffset)
        {
            char* m_String = &m_StringPtr[m_StringOffset];
            m_List.emplace_back(m_Symbols[m_Index], m_String);

            ++m_Index;
            m_StringOffset += (static_cast<uint32_t>(strlen(m_String)) + 1);
        }

        std::sort(m_List.begin(), m_List.end(), CompareKey);
    }

    void UpdateLocalizationString(uint32_t p_Key, const char* p_NewValue)
    {
        UFG::UILocalization_t* m_UILocalization = reinterpret_cast<UFG::UILocalization_t*>(GetResourceData());

        const char* p_Value = nullptr;
        for (auto& m_Pair : m_List)
        {
            if (m_Pair.first == p_Key)
            {
                p_Value = m_Pair.second;
                break;
            }
        }

        size_t m_ValueLen       = strlen(p_Value);
        size_t m_NewValueLen    = strlen(p_NewValue);

        size_t m_NewDataSize = m_DataSize - m_ValueLen + m_NewValueLen;
        void* m_OldDataPtr = m_DataPtr;
        void* m_NewDataPtr = malloc(m_NewDataSize);
        if (!m_NewDataPtr)
            return;

        memset(m_NewDataPtr, 0, m_NewDataSize);
        memcpy(m_NewDataPtr, m_DataPtr, (sizeof(UFG::UILocalization_t) + m_UILocalization->m_SymbolsSize));
        m_DataPtr   = m_NewDataPtr;
        m_DataSize  = m_NewDataSize;

        UFG::UILocalization_t* m_NewUILocalization = reinterpret_cast<UFG::UILocalization_t*>(GetResourceData());
        m_NewUILocalization->m_StringsSize = m_UILocalization->m_StringsSize - static_cast<uint32_t>(m_ValueLen) + static_cast<uint32_t>(m_NewValueLen);

        char* m_StringPtr       = m_UILocalization->GetStringPtr();
        char* m_NewStringPtr    = m_NewUILocalization->GetStringPtr();
        size_t m_StringCopySize    = (reinterpret_cast<size_t>(p_Value) - reinterpret_cast<size_t>(m_StringPtr));
        size_t m_StringCopySize2   = (static_cast<size_t>(m_UILocalization->m_StringsSize) - (m_StringCopySize + m_ValueLen + 1));
        memcpy(m_NewStringPtr, m_StringPtr, m_StringCopySize);
        memcpy(&m_NewStringPtr[m_StringCopySize], p_NewValue, m_NewValueLen);
        memcpy(&m_NewStringPtr[m_StringCopySize + m_NewValueLen + 1], &m_StringPtr[m_StringCopySize + m_ValueLen + 1], m_StringCopySize2);

        m_List.clear();
        free(m_OldDataPtr);

        OnDataLoad();
    }

    struct LocalizationEditValue_t
    {
        std::string m_Name;
        uint32_t m_Key;
        char m_Str[1024];
    };
    LocalizationEditValue_t LocalizationEditValue;

    char m_SearchKeyStr[64] = { '\0' };

    void UpdateSearch()
    {
        m_SearchList.clear();
        if (m_SearchKeyStr[0] != '\0')
        {
            std::string m_SearchKey = m_SearchKeyStr;
            std::transform(m_SearchKey.begin(), m_SearchKey.end(), m_SearchKey.begin(), toupper);

            for (auto& m_Pair : m_List)
            {
                std::string m_KeyName = GetKeyName(m_Pair.first);
                std::transform(m_KeyName.begin(), m_KeyName.end(), m_KeyName.begin(), toupper);

                if (m_KeyName.find(m_SearchKey) != std::string::npos)
                    m_SearchList.emplace_back(m_Pair.first, m_Pair.second);
            }
        }
    }

    void RenderProperties()
    {
        if (ImGui::CollapsingHeader(u8"\uE0BB Dictionary", IMGUI_TREENODE_OPEN_FLAGS))
        {
            ImGui::SetNextItemWidth(-110.f);
            if (ImGui::InputText("Search for Key##UILocalization", m_SearchKeyStr, sizeof(m_SearchKeyStr)))
                UpdateSearch();

            std::vector<std::pair<uint32_t, const char*>>& m_DisplayList = (m_SearchKeyStr[0] == '\0' ? m_List : m_SearchList);
            if (ImGui::BeginTable("##UILocalizationTable", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, { 0.f, -1.f }))
            {
                if (ImGui::BeginPopup("##UILocalizationEditValue", ImGuiWindowFlags_MenuBar))
                {
                    if (ImGui::BeginMenuBar())
                    {
                        ImGui::Text("%s (Edit Value)\t", &LocalizationEditValue.m_Name[0]);
                        ImGui::EndMenuBar();
                    }

                    if (ImGui::InputTextMultiline("##UILocalizationEditValue_Str", LocalizationEditValue.m_Str, sizeof(LocalizationEditValue_t::m_Str), { -1.f, 50.f }))
                    {
                        UpdateLocalizationString(LocalizationEditValue.m_Key, LocalizationEditValue.m_Str);
                        UpdateSearch();
                    }

                    ImGui::EndPopup();
                }

                ImGui::TableSetupScrollFreeze(1, 1);
                ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, floorf(ImGui::GetContentRegionAvail().x * 0.35f));
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                int m_HoveredRow = ImGui::TableGetHoveredRow();
                if (m_HoveredRow && Core_ImGui_RightClickItemPopupNoHover("##UILocalizationEditValue"))
                {
                    auto& m_Pair = m_DisplayList[m_HoveredRow - 1];
                    LocalizationEditValue.m_Name = GetKeyName(m_Pair.first);
                    LocalizationEditValue.m_Key = m_Pair.first;
                    memset(LocalizationEditValue.m_Str, 0, sizeof(LocalizationEditValue_t::m_Str));
                    strncpy(LocalizationEditValue.m_Str, m_Pair.second, sizeof(LocalizationEditValue_t::m_Str));
                }

                for (auto& m_Pair : m_DisplayList)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(GetKeyName(m_Pair.first));

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushTextWrapPos(0.f);
                    ImGui::TextEx(m_Pair.second);
                    ImGui::PopTextWrapPos();
                }

                ImGui::EndTable();
            }
        }
    }
};