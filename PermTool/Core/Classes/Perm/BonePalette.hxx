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
		if (!m_BonePallete)
			return;

		Core_ImGui_TextSuffix("Count", Format::Get("%u", m_BonePallete->m_NumBones));

		if (ImGui::TreeNodeEx(Format::Get(u8"\uF03A List##%u", m_BonePallete->m_NameUID), IMGUI_TREENODE_FLAGS))
		{
			auto m_BoneNameTable = m_BonePallete->GetBoneNameTable();
			uint32_t* m_BoneUIDTable = m_BonePallete->GetBoneUIDTable();
			for (uint32_t i = 0; m_BonePallete->m_NumBones > i; ++i)
			{
				ImGui::BulletText((&m_BoneNameTable[i])->m_Str);
				if (Core_ImGui_ToolTipHover())
				{
					Core_ImGui_TextSuffix("Index", Format::Get("%u", i), 80.f);
					Core_ImGui_TextSuffix("NameUID", Format::Get("0x%X", m_BoneUIDTable[i]), 80.f);
					ImGui::EndTooltip();
				}
			}

			ImGui::TreePop();
		}
	}
};