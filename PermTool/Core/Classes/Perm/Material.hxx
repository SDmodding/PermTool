#pragma once

CQSymbolMap g_MaterialParamSymbolMap = {{
	"texAmbient",
	"texBump", "texBump2",
	"texDiffuse", "texDiffuse2", "texDiffuseBlend", "texDiffuseWorld",
	"texNoise",
	"texNormal",
	"texSpecular",
	"iAlphaState", "iRasterState", "iShader", "iTexture"
}};

std::unordered_map<uint32_t, CQSymbolMap*> g_MaterialParamNameMap =
{
	{ SDK::StringHash32("iAlphaState"), &g_AlphaStateSymbolMap },
	{ SDK::StringHash32("iRasterState"), &g_RasterStateSymbolMap },
	{ SDK::StringHash32("iShader"), &g_ShaderSymbolMap }
};

struct MaterialParamAdd_t
{
	uint32_t m_StateTypeUID;
	uint32_t m_StateNameUID;
	uint32_t m_TypeUID;

	MaterialParamAdd_t() {}
	MaterialParamAdd_t(uint32_t p_StateTypeUID, uint32_t p_StateNameUID, uint32_t p_TypeUID)
	{
		m_StateTypeUID = p_StateTypeUID;
		m_StateNameUID = p_StateNameUID;
		m_TypeUID = p_TypeUID;
	}
};
std::map<std::string, std::map<std::string, MaterialParamAdd_t>> g_MaterialParamAddList =
{
	{ "Texture", {
		{ "Ambient",		{ 0xC8377453, 0x5B6955EA, 0x8B43FABF }},
		{ "Bump",			{ 0xC8377453, 0xADBE1A5A, 0x8B43FABF }},
		{ "Bump (2)",		{ 0xC8377453, 0xA348DC23, 0x8B43FABF }},
		{ "Diffuse",		{ 0xC8377453, 0xDCE06689, 0x8B43FABF }},
		{ "Diffuse (2)",	{ 0xC8377453, 0x19410F73, 0x8B43FABF }},
		{ "Diffuse Blend",	{ 0xC8377453, 0x37D61377, 0x8B43FABF }},
		{ "Diffuse World",	{ 0xC8377453, 0x05F95AF1, 0x8B43FABF }},
		{ "Noise",			{ 0xC8377453, 0x63E8D31B, 0x8B43FABF }},
		{ "Normal",			{ 0xC8377453, 0xECADC789, 0x8B43FABF }},
		{ "Specular",		{ 0xC8377453, 0xCB460EC7, 0x8B43FABF }}
	}}
};

class CMaterial : public CPerm
{
public:
	CMaterial()
	{
		m_HasTreeNodeItems = true;
	}

	void UpdateMaterialParamList(Illusion::Material_t* p_Material, std::vector<Illusion::MaterialParam_t>& p_ParamList)
	{
		size_t m_NewDataSize = (m_DataSize - (static_cast<size_t>(p_Material->m_NumParams) * sizeof(Illusion::MaterialParam_t)));
		m_NewDataSize += (p_ParamList.size() * sizeof(Illusion::MaterialParam_t));

		void* m_OldDataPtr = m_DataPtr;
		void* m_NewDataPtr = malloc(m_NewDataSize);
		if (!m_NewDataPtr)
			return;
		
		ZeroMemory(m_NewDataPtr, m_NewDataSize);
		memcpy(m_NewDataPtr, m_DataPtr, static_cast<size_t>(GetResourceEntry()->m_Offset) + sizeof(Illusion::Material_t));
		m_DataPtr	= m_NewDataPtr;
		m_DataSize	= m_NewDataSize;

		Illusion::Material_t* m_Material = reinterpret_cast<Illusion::Material_t*>(GetResourceData());
		m_Material->m_NumParams = static_cast<uint32_t>(p_ParamList.size());
		m_Material->m_MaterialUserOffset = static_cast<int64_t>(p_ParamList.size() * sizeof(Illusion::MaterialParam_t));

		Illusion::MaterialParam_t* m_ParamTable = m_Material->GetTable();
		for (uint32_t i = 0; m_Material->m_NumParams > i; ++i)
			memcpy(&m_ParamTable[i], &p_ParamList.data()[i], sizeof(Illusion::MaterialParam_t));

		Illusion::MaterialUser_t* m_OldMaterialUser = p_Material->GetUser();
		Illusion::MaterialUser_t* m_MaterialUser = m_Material->GetUser();
		{
			m_MaterialUser->m_VisibilityFlags = m_OldMaterialUser->m_VisibilityFlags;
			m_MaterialUser->mShadowFlags = m_OldMaterialUser->mShadowFlags;
		}

		free(m_OldDataPtr);
	}

	void AddMaterialParam(Illusion::Material_t* p_Material, Illusion::MaterialParam_t p_Param)
	{
		std::vector<Illusion::MaterialParam_t> m_ParamList;

		Illusion::MaterialParam_t* m_ParamTable = p_Material->GetTable();
		for (uint32_t i = 0; p_Material->m_NumParams > i; ++i)
			m_ParamList.emplace_back(*(&m_ParamTable[i]));

		m_ParamList.emplace_back(p_Param);
		UpdateMaterialParamList(p_Material, m_ParamList);
	}

	void RemoveMaterialParam(Illusion::Material_t* p_Material, Illusion::MaterialParam_t* p_Param)
	{
		std::vector<Illusion::MaterialParam_t> m_ParamList;

		// Fill Param list...
		{
			Illusion::MaterialParam_t* m_ParamTable = p_Material->GetTable();
			for (uint32_t i = 0; p_Material->m_NumParams > i; ++i)
			{
				Illusion::MaterialParam_t* m_Param = &m_ParamTable[i];
				if (m_Param == p_Param)
					continue;

				m_ParamList.emplace_back(*m_Param);
			}
		}

		UpdateMaterialParamList(p_Material, m_ParamList);
	}

	void RenderTreeNode()
	{
		Illusion::Material_t* m_Material = reinterpret_cast<Illusion::Material_t*>(GetResourceData());

		if (ImGui::TreeNodeEx(u8"\uE253 Params", IMGUI_TREENODE_OPEN_FLAGS))
		{
			Illusion::MaterialParam_t* m_ParamTable = m_Material->GetTable();
			for (uint32_t i = 0; m_Material->m_NumParams > i; ++i)
			{
				Illusion::MaterialParam_t* m_Param = &m_ParamTable[i];
				const char* m_ParamStateName = g_MaterialParamSymbolMap.Get(m_Param->m_StateParam.m_NameUID, Format::Get("0x%X", m_Param->m_StateParam.m_NameUID));
				const char* m_ParamName = nullptr;

				if (m_Param->m_NameUID == UINT32_MAX)
					m_ParamName = "Default";
				else
				{
					auto m_Find = g_MaterialParamNameMap.find(m_Param->m_StateParam.m_NameUID);
					if (m_Find != g_MaterialParamNameMap.end())
						m_ParamName = (*m_Find).second->Get(m_Param->m_NameUID);
					if (!m_ParamName)
						m_ParamName = Format::GetUIntHex(m_Param->m_NameUID);
				}

				Core_ImGui_TextSuffix(m_ParamStateName, m_ParamName, 200.f);
			}

			ImGui::TreePop();
		}
	}

	struct MaterialParamCtxItem_t
	{
		std::string m_Name;
		Illusion::MaterialParam_t* m_Ptr = nullptr;
		Illusion::MaterialParam_t m_Backup;
		bool m_Uppercase = false;
		char m_Str[128];
	};
	MaterialParamCtxItem_t MaterialParamCtxItem;

	void ImGui_AddMaterialParamSelectable(Illusion::Material_t* p_Material, const char* p_Name, uint32_t p_StateTypeUID, uint32_t p_StateNameUID, uint32_t p_TypeUID)
	{
		if (!ImGui::Selectable(p_Name))
			return;

		Illusion::MaterialParam_t m_Param;
		ZeroMemory(&m_Param, sizeof(m_Param));
		m_Param.m_StateParam.m_TypeUID = p_StateTypeUID;
		m_Param.m_StateParam.m_NameUID = p_StateNameUID;
		m_Param.m_TypeUID = p_TypeUID;
		m_Param.m_NameUID = UINT32_MAX;
		AddMaterialParam(p_Material, m_Param);
	}

	void RenderProperties()
	{
		Illusion::Material_t* m_Material = reinterpret_cast<Illusion::Material_t*>(GetResourceData());

		if (ImGui::BeginPopup("##MaterialParamCtxItem", ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Text(&MaterialParamCtxItem.m_Name[0]);
				ImGui::EndMenuBar();
			}

			auto m_Find = g_MaterialParamNameMap.find(MaterialParamCtxItem.m_Ptr->m_StateParam.m_NameUID);
			if (m_Find != g_MaterialParamNameMap.end())
			{
				auto m_ParamItems = (*m_Find).second->GetSortedVector();
				m_ParamItems.insert(m_ParamItems.begin(), { UINT32_MAX, "Default" });

				ImGui::SeparatorText("Select");
				const char* m_Selected = nullptr;
				for (auto m_Pair : m_ParamItems)
				{
					if (m_Pair.first == MaterialParamCtxItem.m_Ptr->m_NameUID)
					{
						m_Selected = m_Pair.second;
						break;
					}
				}
				if (!m_Selected)
					m_Selected = Format::GetUIntHex(MaterialParamCtxItem.m_Ptr->m_NameUID);

				if (ImGui::BeginCombo("##MaterialParamCtxItem_SelectList", m_Selected))
				{
					for (auto m_Pair : m_ParamItems)
					{
						bool m_Selected = (m_Pair.first == MaterialParamCtxItem.m_Ptr->m_NameUID);
						if (ImGui::Selectable(m_Pair.second, &m_Selected))
							MaterialParamCtxItem.m_Ptr->m_NameUID = m_Pair.first;
					}

					ImGui::EndCombo();
				}		
			}
			else
			{
				ImGui::SeparatorText("Name");
				bool m_Update = ImGui::Checkbox("Uppercase##MaterialParamCtxItem", &MaterialParamCtxItem.m_Uppercase);
				if (ImGui::InputText("##MaterialParamCtxItem_Str", MaterialParamCtxItem.m_Str, sizeof(MaterialParamCtxItem_t::m_Str)) || m_Update)
				{
					if (MaterialParamCtxItem.m_Str[0] == '\0')
						MaterialParamCtxItem.m_Ptr->m_NameUID = UINT32_MAX;
					else if (MaterialParamCtxItem.m_Uppercase)
						MaterialParamCtxItem.m_Ptr->m_NameUID = SDK::StringHashUpper32(MaterialParamCtxItem.m_Str);
					else
						MaterialParamCtxItem.m_Ptr->m_NameUID = SDK::StringHash32(MaterialParamCtxItem.m_Str);
				}
			}

			if (ImGui::Button("Revert##MaterialParamCtxItem", { -1.f, 0.f }))
				MaterialParamCtxItem.m_Ptr->m_NameUID = MaterialParamCtxItem.m_Backup.m_NameUID;

			ImGui::SeparatorText("Options");
			if (ImGui::Button("Remove##MaterialParamCtxItem", { -1.f, 0.f }))
			{
				RemoveMaterialParam(m_Material, MaterialParamCtxItem.m_Ptr);
				m_Material = reinterpret_cast<Illusion::Material_t*>(GetResourceData());;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::CollapsingHeader(u8"\uE253 Params", IMGUI_TREENODE_OPEN_FLAGS))
		{
			Illusion::MaterialParam_t* m_ParamTable = m_Material->GetTable();
			for (uint32_t i = 0; m_Material->m_NumParams > i; ++i)
			{
				Illusion::MaterialParam_t* m_Param = &m_ParamTable[i];
				const char* m_ParamStateName = g_MaterialParamSymbolMap.Get(m_Param->m_StateParam.m_NameUID, Format::Get("0x%X", m_Param->m_StateParam.m_NameUID));
				Core_ImGui_InputUInt(Format::Get("%s##%u.%u", m_ParamStateName, m_Material->m_NameUID, i), &m_Param->m_NameUID, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

				if (Core_ImGui_RightClickItemPopup("##MaterialParamCtxItem"))
				{
					MaterialParamCtxItem.m_Name		= m_ParamStateName;
					MaterialParamCtxItem.m_Ptr		= m_Param;
					MaterialParamCtxItem.m_Backup	= *m_Param;
					ZeroMemory(MaterialParamCtxItem.m_Str, sizeof(MaterialParamCtxItem_t::m_Str));
				}
			}

			if (ImGui::BeginCombo("Add##MaterialParam", "* Select *"))
			{
				for (auto& m_CategoryPair : g_MaterialParamAddList)
				{
					ImGui::SeparatorText(&m_CategoryPair.first[0]);

					for (auto& m_Pair : m_CategoryPair.second)
						ImGui_AddMaterialParamSelectable(m_Material, &m_Pair.first[0], m_Pair.second.m_StateTypeUID, m_Pair.second.m_StateNameUID, m_Pair.second.m_TypeUID);
				}

				ImGui::EndCombo();
			}
		}
	}
};