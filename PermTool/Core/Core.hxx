#pragma once

// Defines
#define IMGUI_COLOR_TEXT2               IM_COL32(150, 150, 150, 255)
#define IMGUI_TREENODE_FLAGS            (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow)
#define IMGUI_TREENODE_OPEN_FLAGS       (IMGUI_TREENODE_FLAGS | ImGuiTreeNodeFlags_DefaultOpen)

// SDK
#define UFG_MAX(a, b) max(a, b)
#define UFG_PAD_INSERT(x, y) x ## y
#define UFG_PAD_DEFINE(x, y) UFG_PAD_INSERT(x, y)
#define UFG_PAD(size) char UFG_PAD_DEFINE(padding_, __LINE__)[size] = { 0x0 }
#include <SDK/Optional/PermFile/.Includes.hpp>
#include <SDK/Optional/StringHash.hpp>

// Includes
#include "Format.hxx"

// Functions
void Core_ImGui_ColorInfo(std::string p_Label, float* p_FloatColor);
bool Core_ImGui_InputUInt(const char* p_Label, uint32_t* p_Value, uint32_t p_Step = 1, uint32_t p_StepFast = 100, ImGuiInputTextFlags p_Flags = ImGuiInputTextFlags_None);
__inline bool Core_ImGui_InputHex(const char* p_Label, uint32_t* p_Value, uint32_t p_Step = 0x1, uint32_t p_StepFast = 0x100)
{
    return Core_ImGui_InputUInt(p_Label, p_Value, p_Step, p_StepFast, ImGuiInputTextFlags_CharsHexadecimal);
}
void Core_ImGui_TextSuffix(const char* p_Prefix, const char* p_Suffix, float p_StartX = 0.f);
bool Core_ImGui_ToolTipHover();
bool Core_ImGui_RightClickItemPopup(const char* p_StrID = nullptr);
bool Core_ImGui_RightClickItemPopupNoHover(const char* p_StrID);
void Core_ImGui_ResourceHandleSelectable(const char* p_Name, uint32_t p_NameUID);
void Core_SelectResourceName(uint32_t p_NameUID);
class CResourceData* Core_FindResourceByName(uint32_t p_NameUID);
void Core_ConvertFloat4Color2UInt(float* p_Floats, uint8_t* p_UInt);

// Global Classes
#include "GClasses/Args.hxx"
#include "GClasses/Configuration.hxx"
#include "GClasses/PopupHandler.hxx"

// Classes Important
#include "Classes/QSymbolMap.hxx"
#include "Classes/ResourceData.hxx"

// Symbols
#include "Symbols/AlphaState.hxx"
#include "Symbols/RasterState.hxx"
#include "Symbols/Shader.hxx"

// Classes
#include "Classes/Perm/.Perm.hxx"

// Helpers
#include "Helpers/UIScreen.hxx"

// Core
class CCore
{
public:
    CPerm* m_PermSelected = nullptr;
	std::vector<std::unique_ptr<CPerm>> m_Perms; // Main list for each entry inside Perm file.
    int m_ImGuiReset = 0; // This will reset some ImGui stuff in multiple windows...

    char m_SearchStr[64] = { '\0' };
    bool m_SearchUpdate = false;

    void SearchPerms()
    {
        m_SearchUpdate = false;

        if (m_SearchStr[0] == '\0')
        {
            for (auto& m_PermPtr : m_Perms)
                m_PermPtr.get()->m_ShowInTree = true;

            return;
        }

        std::string m_SearchKey = m_SearchStr;
        std::transform(m_SearchKey.begin(), m_SearchKey.end(), m_SearchKey.begin(), toupper);

        for (auto& m_PermPtr : m_Perms)
        {
            CPerm* m_Perm = m_PermPtr.get();
            UFG::ResourceEntry_t* m_ResourceEntry = m_Perm->GetResourceEntry();
            if (!m_ResourceEntry)
                continue;

            CResourceData* m_ResourceData = reinterpret_cast<CResourceData*>(m_ResourceEntry->GetData());
            if (!m_ResourceData)
                continue;

            std::string m_PermName = m_ResourceData->m_DebugName;
            std::transform(m_PermName.begin(), m_PermName.end(), m_PermName.begin(), toupper);

            m_Perm->m_ShowInTree = (m_PermName.find(m_SearchKey) != std::string::npos);
        }
    }

    void Reset()
    {
        m_PermSelected = nullptr;
        m_Perms.clear();
        m_ImGuiReset = 2; // Num of windows we use...
        m_SearchUpdate = m_SearchStr[0] != '\0';
    }

    struct DragDrop_t
    {
        const char* m_Name = "PERM_DRAG_DROP";
        size_t m_Source = 0, m_Destination = 0;
    };
    DragDrop_t m_DragDrop;

    // Perm

    std::unique_ptr<CPerm> GetPermResource(UFG::ResourceEntry_t* m_Resource)
    {
        CPerm* m_Perm = Perm::Get(m_Resource->m_TypeUID);
        m_Perm->InitializeData(m_Resource, static_cast<size_t>(m_Resource->GetEntrySize()));
        m_Perm->OnDataLoad();
        return std::unique_ptr<CPerm>(m_Perm);
    }

    bool LoadPermFile(const char* p_FilePath, bool p_Import)
    {
        SDK::PermFile_t m_PermFile;
        if (!m_PermFile.LoadFile(p_FilePath))
            return false;

        if (!p_Import)
        {
            SetWindowTextA(g_Window, Format::Get("%s (%s)", PROJECT_NAME, &m_PermFile.m_Name[0]));
            Reset();
        }

        for (UFG::ResourceEntry_t* m_Resource : m_PermFile.m_Resources)
            m_Perms.emplace_back(GetPermResource(m_Resource));

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

    const char* OpenPermFile(bool p_Import)
    {
        OPENFILENAMEA* m_OpenFileName = GetPermOpenFileName();
        m_OpenFileName->lpstrTitle = (p_Import ? "Import Perm File" : "Open Perm File");
        if (GetOpenFileNameA(m_OpenFileName) == 0)
        {
            m_OpenFileName->lpstrFile[0] = '\0';
            return nullptr;
        }

        if (!HasPathBinOrPermExtension(m_OpenFileName->lpstrFile))
            return "This is not Perm file!";

        if (!LoadPermFile(m_OpenFileName->lpstrFile, p_Import))
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

    bool ExportPermResource(CPerm* p_Perm)
    {
        OPENFILENAMEA* m_OpenFileName = GetPermOpenFileName();
        m_OpenFileName->lpstrTitle = "Export Resource File";
        m_OpenFileName->lpstrFilter = "(Bin File)\0*.bin\0";
        if (GetSaveFileNameA(m_OpenFileName) == 0)
        {
            m_OpenFileName->lpstrFile[0] = '\0';
            return true;
        }

        std::string m_FilePath = m_OpenFileName->lpstrFile;
        if (!HasPathBinOrPermExtension(m_OpenFileName->lpstrFile))
            m_FilePath += ".bin";

        FILE* m_File = fopen(&m_FilePath[0], "wb");
        if (!m_File)
            return false;

        fwrite(p_Perm->m_DataPtr, 1, p_Perm->m_DataSize, m_File);
        fclose(m_File);
        return true;
    }

    const char* ReplaceSelectedPermResource()
    {
        OPENFILENAMEA* m_OpenFileName = GetPermOpenFileName();
        m_OpenFileName->lpstrTitle = "Open Resource File";
        m_OpenFileName->lpstrFilter = "(Bin File)\0*.bin\0";
        if (GetOpenFileNameA(m_OpenFileName) == 0)
        {
            m_OpenFileName->lpstrFile[0] = '\0';
            return nullptr;
        }

        if (!HasPathBinOrPermExtension(m_OpenFileName->lpstrFile))
            return "This is not Resource file!";

        SDK::PermFile_t m_PermFile;
        if (!m_PermFile.LoadFile(m_OpenFileName->lpstrFile))
            return "Failed to open/read Resource file!";

        if (m_PermFile.m_Resources.empty())
            return "No Resource found!";

        std::unique_ptr<CPerm> m_PermResourcePtr = GetPermResource(m_PermFile.m_Resources[0]);
        CPerm* m_PermResource = m_PermResourcePtr.get();

        CResourceData* m_ResourceData = m_PermResource->GetResourceData();
        if (!m_ResourceData)
            return "No ResourceData found!";

        CResourceData* m_ReplaceResourceData = m_PermSelected->GetResourceData();
        if (m_ResourceData->m_TypeUID != m_ReplaceResourceData->m_TypeUID || m_ResourceData->m_ChunkUID != m_ReplaceResourceData->m_ChunkUID)
            return "TypeUID/ChunkUID mismatch!";

        for (auto& m_Perm : m_Perms)
        {
            if (m_Perm.get() == m_PermSelected)
                m_Perm.swap(m_PermResourcePtr);
        }

        m_PermSelected = m_PermResource;
        return nullptr;
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

    static bool Render_RemovePopup(CCore* p_Core)
    {
        ImGui::TextWrapped("Are you sure you want to remove: '%s'?", p_Core->m_PermSelected->GetResoruceName());

        float m_ButtonWidth = floorf(ImGui::GetContentRegionAvail().x * 0.5f) - 4.f;

        if (ImGui::Button("Yes", { m_ButtonWidth, 0.f }))
        {
            for (auto m_It = p_Core->m_Perms.begin(); m_It != p_Core->m_Perms.end(); ++m_It)
            {
                if ((*m_It).get() == p_Core->m_PermSelected)
                {
                    p_Core->m_Perms.erase(m_It);
                    break;
                }
            }

            p_Core->m_PermSelected = nullptr;
            return true;
        }

        ImGui::SameLine();

        if (ImGui::Button("No", { m_ButtonWidth, 0.f }))
            return true;

        return false;
    }

	void RenderTree()
	{
        if (m_ImGuiReset)
        {
            --m_ImGuiReset;
            ImGui_ResetStates();
        }

        ImGui::SetNextItemWidth(-50.f);
        {
            if (ImGui::InputText("Search##Tree", m_SearchStr, sizeof(m_SearchStr)) || m_SearchUpdate)
                SearchPerms();
        }
        ImGui::Separator();

        size_t m_Count = m_Perms.size();
        for (size_t i = 0; m_Count > i; ++i)
        {
            CPerm* m_Perm = m_Perms[i].get();
            if (!m_Perm->m_ShowInTree)
                continue;

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

            bool m_ShouldSelectResource = (m_SelectResourceNameUID == m_ResourceData->m_NameUID);
            if (m_ShouldSelectResource)
            {
                m_PermSelected = m_Perm;
                m_SelectResourceNameUID = UINT32_MAX;
                ImGui::SetNextItemOpen(true);
            }

            const char* m_ResourceNodeID = Format::Get("##%u.%zu", m_ResourceData->m_NameUID, i);
            std::string m_ResourceName = m_ResourceData->GetName();
            bool m_TreeNodeOpen = ImGui::TreeNodeEx(m_ResourceNodeID, m_TreeNodeFlags | ImGuiTreeNodeFlags_SpanAvailWidth, "%s %s", m_ResourceTypeInfo->m_Icon, m_ResourceName.c_str());
            ImGui::PushID(m_ResourceNodeID);

            if (m_ShouldSelectResource)
                ImGui::SetScrollHereY(0.f);

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

            if (Core_ImGui_RightClickItemPopup("##PermCtxOptions"))
                m_PermSelected = m_Perm;

            if (ImGui::BeginPopupContextItem("##PermCtxOptions"))
            {
                if (m_PermSelected)
                {
                    if (m_PermSelected->m_HasCtxOptions)
                    {
                        m_PermSelected->RenderCtxOptions();
                        ImGui::Separator();
                    }

                    if (ImGui::MenuItem(u8"\uF361 Replace"))
                    {
                        const char* m_Result = ReplaceSelectedPermResource();
                        if (m_Result)
                            g_PopupHandler.AddText(g_PopupErrorTitle, m_Result);
                    }

                    if (ImGui::MenuItem(u8"\uF56E Export"))
                        ExportPermResource(m_PermSelected);

                    if (ImGui::MenuItem(u8"\uF1F8 Remove"))
                        g_PopupHandler.AddCallback(u8"\uF1F8 Remove", { 400.f, -1.f }, Render_RemovePopup, this);
                }
                else
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoPreviewTooltip))
            {
                ImGui::SetDragDropPayload(m_DragDrop.m_Name, &i, sizeof(size_t));
                ImGui::EndDragDropSource();

                if (g_Configuration.Warning.CanShowMoveResource())
                    g_PopupHandler.AddText(u8"\uF071 Warning", "Moving resources around can cause issues when you don't know what you're doing!");
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* m_PayLoad = ImGui::AcceptDragDropPayload(m_DragDrop.m_Name, ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
                {
                    m_DragDrop.m_Source = *reinterpret_cast<size_t*>(m_PayLoad->Data);
                    m_DragDrop.m_Destination = i;
                }

                ImGui::EndDragDropTarget();
            }

            if (m_TreeNodeOpen)
            {
                m_Perm->RenderTreeNode();
                ImGui::TreePop();
            }

            ImGui::PopID();
        }

        if (m_DragDrop.m_Source != m_DragDrop.m_Destination)
        {
            if (m_DragDrop.m_Source > m_DragDrop.m_Destination)
            {
                size_t m_NumToMove = (m_DragDrop.m_Source - m_DragDrop.m_Destination);
                for (size_t i = 0; m_NumToMove > i; ++i)
                    m_Perms[m_DragDrop.m_Source - i].swap(m_Perms[m_DragDrop.m_Source - i - 1]);
            }
            else
            {
                size_t m_NumToMove = (m_DragDrop.m_Destination - m_DragDrop.m_Source);
                for (size_t i = 0; m_NumToMove > i; ++i)
                    m_Perms[m_DragDrop.m_Source + i].swap(m_Perms[m_DragDrop.m_Source + i + 1]);
            }

            ImGui::SetDragDropPayload(m_DragDrop.m_Name, &m_DragDrop.m_Destination, sizeof(size_t));
            m_DragDrop.m_Source = m_DragDrop.m_Destination = 0;
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
        {
            m_PermSelected->RenderDefaultProperties();
            m_PermSelected->RenderProperties();
        }
    }

    // CMode (Command Line)

    HANDLE m_StdOutputHandle = 0;
    CONSOLE_SCREEN_BUFFER_INFO m_ConsoleBufferInfo = { 0 };

    __inline bool CMode_Silent()
    {
        return (g_Args.IsSet("silent"));
    }

    bool CMode_Attach()
    {
        if (CMode_Silent())
            return true;

        if (!AttachConsole(ATTACH_PARENT_PROCESS))
            return false;

        // Open std outputs...
        {
            freopen("CONIN$", "r", stdin);
            freopen("CONOUT$", "w", stderr);
            freopen("CONOUT$", "w", stdout);
        }

        m_StdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(m_StdOutputHandle, &m_ConsoleBufferInfo);
        return true;
    }

    void CMode_Detach()
    {
        if (CMode_Silent())
            return;

        // TODO: Somehow tell retard Windows to go back to user cmd because cmd is waiting for user to press enter/ctrl+c to exit from attached console...
        fclose(stdin);
        fclose(stderr);
        fclose(stdout);

        FreeConsole();
    }

    void CMode_Syntax()
    {
        std::string m_ModuleName(MAX_PATH, '\0');
        if (GetModuleFileNameA(0, &m_ModuleName[0], static_cast<DWORD>(m_ModuleName.size())) == 0)
            m_ModuleName = "PermTool.exe";
        else
            m_ModuleName = std::filesystem::path(m_ModuleName).filename().string();

        printf("[ Syntax ] %s -cmode infile -options\n", &m_ModuleName[0]);
    }

    void CMode_Options()
    {

    }

    // Callback from WinMain when running in CMode
    void CMode_Main(char* p_PermFile)
    {
        if (!CMode_Silent())
        {
            SetConsoleTextAttribute(m_StdOutputHandle, 240); 
            printf("PermTool console mode activated!\n"); 
            SetConsoleTextAttribute(m_StdOutputHandle, m_ConsoleBufferInfo.wAttributes);
        }

        if (!p_PermFile || 4 > g_Args.m_Count)
        {
            printf("[ ! ] Invalid parameters specified.\n");
            CMode_Syntax();
            CMode_Options();
            return;
        }
    }
};
CCore g_Core;

#include "CoreFunctions.hxx"