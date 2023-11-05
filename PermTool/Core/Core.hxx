#pragma once

// Defines
#define IMGUI_COLOR_TEXT2               IM_COL32(150, 150, 150, 255)
#define IMGUI_TREENODE_FLAGS            (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow)
#define IMGUI_TREENODE_OPEN_FLAGS       (IMGUI_TREENODE_FLAGS | ImGuiTreeNodeFlags_DefaultOpen)

// SDK
#define UFG_PAD_INSERT(x, y) x ## y
#define UFG_PAD_DEFINE(x, y) UFG_PAD_INSERT(x, y)
#define UFG_PAD(size) char UFG_PAD_DEFINE(padding_, __LINE__)[size] = { 0x0 }
#include <SDK/Optional/PermFile/.Includes.hpp>
#include <SDK/Optional/StringHash.hpp>

// Includes
#include "Format.hxx"

// Functions
void Core_ImGui_TextSuffix(const char* p_Prefix, const char* p_Suffix, float p_StartX = 0.f);
bool Core_ImGui_ToolTipHover();
bool Core_ImGui_RightClickItemPopup(const char* p_StrID = nullptr);
bool Core_ImGui_RightClickItemPopupNoHover(const char* p_StrID);
void Core_ImGui_ResourceHandleSelectable(const char* p_Name, uint32_t p_NameUID, uint32_t p_OwnerUID);
void Core_SelectResourceName(uint32_t p_NameUID);
class CResourceData* Core_FindResourceByName(uint32_t p_NameUID);

// Classes
#include "Classes/QSymbolMap.hxx"
#include "Classes/ResourceData.hxx"
#include "Classes/Perm/.Perm.hxx"

// Core
class CCore
{
public:
    CPerm* m_PermSelected = nullptr;
	std::vector<std::unique_ptr<CPerm>> m_Perms; // Main list for each entry inside Perm file.
    int m_ImGuiReset = 0; // This will reset some ImGui stuff in multiple windows...

    void Reset()
    {
        m_PermSelected = nullptr;
        m_Perms.clear();
        m_ImGuiReset = 2; // Num of windows we use...
    }

    // Perm

    void AddPermResource(UFG::ResourceEntry_t* m_Resource)
    {
        CPerm* m_Perm = Perm::Get(m_Resource->m_TypeUID);
        m_Perm->InitializeData(m_Resource, static_cast<size_t>(m_Resource->GetEntrySize()));
        m_Perm->OnDataLoad();
        m_Perms.emplace_back(std::unique_ptr<CPerm>(m_Perm));
    }

    bool LoadPermFile(const char* p_FilePath)
    {
        SDK::PermFile_t m_PermFile;
        if (!m_PermFile.LoadFile(p_FilePath))
            return false;

        SetWindowTextA(g_Window, Format::Get("%s (%s)", PROJECT_NAME, &m_PermFile.m_Name[0]));
        Reset();

        for (UFG::ResourceEntry_t* m_Resource : m_PermFile.m_Resources)
            AddPermResource(m_Resource);

        return true;
    }

    char m_PermFilePath[MAX_PATH] = { '\0' };
    bool HasPathBinOrPermExtension(const char* p_FilePath)
    {
        std::string m_FilePath = p_FilePath;
        std::transform(m_FilePath.begin(), m_FilePath.end(), m_FilePath.begin(), tolower);
        if (m_FilePath.find(".bin") == std::string::npos && m_FilePath.find(".perm.bin") == std::string::npos || m_FilePath.find(".temp.bin") != std::string::npos)
            return false;

        return true;
    }

    OPENFILENAMEA* GetPermOpenFileName()
    {
        static OPENFILENAMEA m_OpenFileName = { 0 };
        {
            ZeroMemory(&m_OpenFileName, sizeof(OPENFILENAMEA));
            m_OpenFileName.lStructSize = sizeof(OPENFILENAMEA);
        }

        static char m_FilePath[MAX_PATH] = { '\0' };
        m_OpenFileName.lpstrFile    = m_FilePath;
        m_OpenFileName.nMaxFile     = sizeof(m_FilePath);
        m_OpenFileName.lpstrFilter  = "(Perm File)\0*.bin\0";
        m_OpenFileName.Flags        = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
        m_OpenFileName.hwndOwner    = g_Window;

        return &m_OpenFileName;
    }

    const char* OpenPermFile()
    {
        OPENFILENAMEA* m_OpenFileName = GetPermOpenFileName();
        m_OpenFileName->lpstrTitle = "Open Perm File";
        if (GetOpenFileNameA(m_OpenFileName) == 0)
        {
            m_OpenFileName->lpstrFile[0] = '\0';
            return nullptr;
        }

        if (!HasPathBinOrPermExtension(m_OpenFileName->lpstrFile))
            return "This is not Perm file!";

        if (!LoadPermFile(m_OpenFileName->lpstrFile))
            return "Failed to open/read Perm file!";

        return nullptr;
    }

    bool SavePermFile()
    {
        OPENFILENAMEA* m_OpenFileName = GetPermOpenFileName();
        m_OpenFileName->lpstrTitle = "Save Perm File";
        if (GetSaveFileNameA(m_OpenFileName) == 0)
        {
            m_OpenFileName->lpstrFile[0] = '\0';
            return true;
        }

        std::string m_FilePath = m_OpenFileName->lpstrFile;
        if (!HasPathBinOrPermExtension(m_OpenFileName->lpstrFile))
            m_FilePath += ".perm.bin";

        FILE* m_File = fopen(&m_FilePath[0], "wb");
        if (!m_File)
            return false;

        for (auto& m_PermPtr : m_Perms)
        {
            CPerm* m_Perm = m_PermPtr.get();
            fwrite(m_Perm->m_DataPtr, 1, m_Perm->m_DataSize, m_File);
        }

        fclose(m_File);
        return true;
    }

    void ExportPermFileDialog()
    {
        IFileDialog* m_FileDialog = nullptr;
        if (CoCreateInstance(CLSID_FileOpenDialog, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_FileDialog)) != S_OK)
            return;

        DWORD m_Options = 0x0;
        if (m_FileDialog->GetOptions(&m_Options) == S_OK)
        {
            m_FileDialog->SetOptions(m_Options | FOS_PICKFOLDERS);
            m_FileDialog->SetTitle(L"Select Folder for Export");

            if (m_FileDialog->Show(g_Window) == S_OK)
            {
                IShellItem* m_ShellItem;
                if (m_FileDialog->GetResult(&m_ShellItem) == S_OK)
                {
                    wchar_t* m_FolderPath = nullptr;
                    m_ShellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &m_FolderPath);

                    size_t m_Count = m_Perms.size();
                    for (size_t i = 0; m_Count > i; ++i)
                    {
                        CPerm* m_Perm = m_Perms[i].get();
                        UFG::ResourceEntry_t* m_ResourceEntry = m_Perm->GetResourceEntry();
                        if (!m_ResourceEntry)
                            continue;

                        CResourceData* m_ResourceData = reinterpret_cast<CResourceData*>(m_ResourceEntry->GetData());
                        if (!m_ResourceData)
                            continue;

                        ResourceTypeInfo_t* m_ResourceTypeInfo = Resource::GetTypeInfo(m_ResourceEntry->m_TypeUID);

                        wchar_t m_FileName[MAX_PATH];
                        swprintf_s(m_FileName, IM_ARRAYSIZE(m_FileName), L"%zu %hs (%hs).bin", i, m_ResourceData->m_DebugName, m_ResourceTypeInfo->m_Name);
                        PathCleanupSpec(m_FileName, m_FileName);

                        FILE* m_File = fopen(Format::Get("%ws\\%ws", m_FolderPath, m_FileName), "wb");
                        if (!m_File)
                            continue;

                        fwrite(m_Perm->m_DataPtr, 1, m_Perm->m_DataSize, m_File);
                        fclose(m_File);
                    }

                    m_ShellItem->Release();
                }
            }
        }

        m_FileDialog->Release();
    }

    // ImGui
    void ImGui_ResetStates()
    {
        ImGuiWindow* m_ImGuiWindow = ImGui::GetCurrentWindow();
        if (!m_ImGuiWindow)
            return;

        m_ImGuiWindow->DC.StateStorage->Clear();
    }

    // Render

    uint32_t m_SelectResourceNameUID = UINT32_MAX;

	void RenderTree()
	{
        if (m_ImGuiReset)
        {
            --m_ImGuiReset;
            ImGui_ResetStates();
        }

        size_t m_Count = m_Perms.size();
        for (size_t i = 0; m_Count > i; ++i)
        {
            CPerm* m_Perm = m_Perms[i].get();
            UFG::ResourceEntry_t* m_ResourceEntry = m_Perm->GetResourceEntry();
            if (!m_ResourceEntry)
                continue;

            CResourceData* m_ResourceData = reinterpret_cast<CResourceData*>(m_ResourceEntry->GetData());
            if (!m_ResourceData)
                continue;

            ResourceTypeInfo_t* m_ResourceTypeInfo = Resource::GetTypeInfo(m_ResourceEntry->m_TypeUID);

            ImGuiTreeNodeFlags m_TreeNodeFlags = IMGUI_TREENODE_FLAGS;
            if (m_PermSelected == m_Perm)
                m_TreeNodeFlags |= ImGuiTreeNodeFlags_Selected;
            if (!m_Perm->m_HasTreeNodeItems)
                m_TreeNodeFlags |= ImGuiTreeNodeFlags_Leaf;

            if (m_SelectResourceNameUID == m_ResourceData->m_NameUID)
            {
                m_PermSelected = m_Perm;
                m_SelectResourceNameUID = UINT32_MAX;
                ImGui::SetScrollHereY(0.f);
                ImGui::SetNextItemOpen(true);
            }

            bool m_TreeNodeOpen = ImGui::TreeNodeEx(Format::Get("%s %s##%u.%zu", m_ResourceTypeInfo->m_Icon, m_ResourceData->m_DebugName, m_ResourceData->m_NameUID, i), m_TreeNodeFlags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_PermSelected = m_Perm;

            if (Core_ImGui_ToolTipHover())
            {
                std::pair<const char*, std::string> m_ResourceInfoList[] =
                {
                    { "Type",       m_ResourceTypeInfo->m_Name },
                    { "NameUID",    Format::Get("0x%X", m_ResourceData->m_NameUID) },
                    { "Size",       Format::Get("%u (0x%X)", m_ResourceData->GetEntrySize(), m_ResourceData->GetEntrySize()) },
                };
                for (auto& m_Pair : m_ResourceInfoList)
                {
                    ImGui::Text("%s:", m_Pair.first);
                    ImGui::SameLine(80.f);
                    ImGui::PushStyleColor(ImGuiCol_Text, IMGUI_COLOR_TEXT2);
                    ImGui::Text(&m_Pair.second[0]);
                    ImGui::PopStyleColor();
                }

                ImGui::EndTooltip();
            }

            if (m_TreeNodeOpen)
            {
                m_Perm->RenderTreeNode();
                ImGui::TreePop();
            }
        }
	}

    void RenderProperties()
    {
        if (m_ImGuiReset)
        {
            --m_ImGuiReset;
            ImGui_ResetStates();
        }

        if (m_PermSelected)
            m_PermSelected->RenderProperties();
    }
};
CCore g_Core;

#include "CoreFunctions.hxx"