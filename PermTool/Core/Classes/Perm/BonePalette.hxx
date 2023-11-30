#pragma once

class CBonePalette : public CPerm
{
public:
	CBonePalette()
	{
		m_HasTreeNodeItems = true;
	}

	void RenderTreeNode()
	{
		Illusion::BonePallete_t* m_BonePallete = reinterpret_cast<Illusion::BonePallete_t*>(GetResourceData());

		Core_ImGui_TextSuffix("Count", Format::GetUInt(m_BonePallete->m_NumBones));

		if (ImGui::TreeNodeEx(u8"\uF03A List", IMGUI_TREENODE_FLAGS))
		{
			auto m_BoneNameTable = m_BonePallete->GetBoneNameTable();
			uint32_t* m_BoneUIDTable = m_BonePallete->GetBoneUIDTable();
			for (uint32_t i = 0; m_BonePallete->m_NumBones > i; ++i)
			{
				ImGui::BulletText((&m_BoneNameTable[i])->m_Str);
				if (Core_ImGui_ToolTipHover())
				{
					Core_ImGui_TextSuffix("Index", Format::GetUInt(i), 80.f);
					Core_ImGui_TextSuffix("NameUID", Format::GetUIntHex(m_BoneUIDTable[i]), 80.f);
					ImGui::EndTooltip();
				}
			}

			ImGui::TreePop();
		}
	}
};