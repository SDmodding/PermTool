//=============================================================================
//
// Description:		Main include file for perm classes handling and other
//					related stuff around perm...
// 
//=============================================================================
#pragma once

struct PermSetNameUID_t
{
	uint32_t* m_Ptr = nullptr;
	uint32_t m_Backup = 0;
	bool m_Uppercase = false;
	char m_Str[128] = { '\0' };
};
PermSetNameUID_t g_PermSetNameUID;

class CPerm
{
public:
	void* m_DataPtr = nullptr;
	size_t m_DataSize = 0;

	bool m_ShowInTree = true;
	bool m_HasCtxOptions = false;
	bool m_HasTreeNodeItems = false;

	virtual ~CPerm() 
	{
		if (m_DataPtr)
		{
			free(m_DataPtr);
			m_DataPtr = nullptr;
		}
	}

	CPerm() { }

	bool InitializeData(void* p_Ptr, size_t p_Size)
	{
		m_DataPtr = malloc(p_Size);
		if (!m_DataPtr)
			return false;

		m_DataSize = p_Size;
		memcpy(m_DataPtr, p_Ptr, p_Size);
		return true;
	}

	// Resource Functions

	__inline UFG::ResourceEntry_t* GetResourceEntry() { return reinterpret_cast<UFG::ResourceEntry_t*>(m_DataPtr); }

	CResourceData* GetResourceData()
	{
		UFG::ResourceEntry_t* m_ResourceEntry = GetResourceEntry();
		if (m_ResourceEntry)
			return reinterpret_cast<CResourceData*>(m_ResourceEntry->GetData());

		return nullptr;
	}

	const char* GetName()
	{
		CResourceData* m_ResourceData = GetResourceData();
		if (!m_ResourceData)
			return "Unknown";

		return m_ResourceData->GetName();
	}

	void RenderDefaultProperties()
	{
		CResourceData* m_ResourceData = GetResourceData();

		if (ImGui::BeginPopup("##NameUIDSetName", ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("NameUID (Set Name)");
				ImGui::EndMenuBar();
			}

			bool m_Update = ImGui::Checkbox("Uppercase##NameUIDSetName", &g_PermSetNameUID.m_Uppercase);
			ImGui::PushItemWidth(280.f);
			if (ImGui::InputText("##NameUIDSetName_Str", g_PermSetNameUID.m_Str, sizeof(PermSetNameUID_t::m_Str)) || m_Update)
			{
				if (g_PermSetNameUID.m_Str[0] == '\0')
					*g_PermSetNameUID.m_Ptr = UINT32_MAX;
				else if (g_PermSetNameUID.m_Uppercase)
					*g_PermSetNameUID.m_Ptr = SDK::StringHashUpper32(g_PermSetNameUID.m_Str);
				else
					*g_PermSetNameUID.m_Ptr = SDK::StringHash32(g_PermSetNameUID.m_Str);
			}

			if (ImGui::Button("Revert##NameUIDSetName", { -1.f, 0.f }))
				*g_PermSetNameUID.m_Ptr = g_PermSetNameUID.m_Backup;

			ImGui::EndPopup();
		}

		if (ImGui::CollapsingHeader(u8"\uF573 Entry Info", IMGUI_TREENODE_FLAGS))
		{
			ImGui::PushItemWidth(-80.f);

			Core_ImGui_InputHex("NameUID", &m_ResourceData->m_NameUID);

			if (Core_ImGui_RightClickItemPopup("##NameUIDSetName"))
			{
				g_PermSetNameUID.m_Ptr = &m_ResourceData->m_NameUID;
				g_PermSetNameUID.m_Backup = m_ResourceData->m_NameUID;
				ZeroMemory(g_PermSetNameUID.m_Str, sizeof(PermSetNameUID_t::m_Str));
				strncpy(g_PermSetNameUID.m_Str, m_ResourceData->m_DebugName, sizeof(PermSetNameUID_t::m_Str));
			}

			if (Core_ImGui_ToolTipHover())
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "WARNING: Modifying this value might be critical to the game and break some stuff if you don't know what you're doing!");
				ImGui::Text("Each resource uses NameUID as identifier/reference, that means that some resource might have handle to this NameUID.");
				ImGui::EndTooltip();
			}

			if (ImGui::InputText("Debug Name", m_ResourceData->m_DebugName, sizeof(CResourceData::m_DebugName)))
			{
				size_t m_NumChars = strlen(m_ResourceData->m_DebugName);
				size_t m_SizeToZero = (sizeof(CResourceData::m_DebugName) - m_NumChars);
				ZeroMemory(&m_ResourceData->m_DebugName[m_NumChars], m_SizeToZero);
			}

			ImGui::SetItemTooltip("You can change this to whatever you want it doesn't do anything.\nIt's mainly used for debugging purposes...");		
			ImGui::PopItemWidth();
		}
	}

	// Virtual Functions

	virtual void OnDataLoad() { }

	virtual void RenderCtxOptions() { }
	virtual void RenderTreeNode() { }
	virtual void RenderProperties() { }
};

// Perm Types
#include "BonePalette.hxx"
#include "Buffer.hxx"
#include "Material.hxx"
#include "ModelData.hxx"
#include "Texture.hxx"
#include "UILocalization.hxx"
#include "UIScreen.hxx"

// Functions
namespace Perm
{
	CPerm* Get(uint32_t p_TypeUID)
	{
		switch (p_TypeUID)
		{
		case RESOURCE_TYPE_BonePalette:
			return new CBonePalette;
		case RESOURCE_TYPE_Buffer:
			return new CBuffer;
		case RESOURCE_TYPE_Material:
			return new CMaterial;
		case RESOURCE_TYPE_ModelData:
			return new CModelData;
		case RESOURCE_TYPE_Texture:
			return new CTexture;
		case RESOURCE_TYPE_UILocalization:
			return new CUILocalization;
		case RESOURCE_TYPE_UIScreen:
			return new CUIScreen;
		default:
			return new CPerm;
		}
	}

	__inline size_t GetAlignSize(size_t p_Size)
	{
		size_t m_Align = (p_Size % 0x10);
		if (p_Size)
			p_Size += (0x10 - m_Align);

		return p_Size;
	}
}