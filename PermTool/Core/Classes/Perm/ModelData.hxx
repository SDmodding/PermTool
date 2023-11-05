#pragma once

CQSymbolMap g_VertexDeclSymbolMap = {{
	"VertexDecl.CloudWisp",
	"VertexDecl.Decal",
	"VertexDecl.Dynamic",
	"VertexDecl.FBX",
	"VertexDecl.FBX2",
	"VertexDecl.GeoParticle",
	"VertexDecl.LightCard",
	"VertexDecl.Lightning",
	"VertexDecl.MorphTarget",
	"VertexDecl.MultiMorphTarget",
	"VertexDecl.NC",
	"VertexDecl.Particle",
	"VertexDecl.Rain",
	"VertexDecl.ScreenParticle",
	"VertexDecl.Skinned",
	"VertexDecl.SkinnedDecal",
	"VertexDecl.SkinnedUVNT",
	"VertexDecl.Slim",
	"VertexDecl.SlimUV",
	"VertexDecl.UV2",
	"VertexDecl.UV2C",
	"VertexDecl.UV2N",
	"VertexDecl.UV2NC",
	"VertexDecl.UV2NT",
	"VertexDecl.UV2NTC",
	"VertexDecl.UVC",
	"VertexDecl.UVCNPacked",
	"VertexDecl.UVN",
	"VertexDecl.UVNPacked",
	"VertexDecl.UVNT",
	"VertexDecl.UVNTC",
	"VertexDecl.VehicleGlassUV2NTC",
	"VertexDecl.VehicleUV2NTC",
	"VertexDecl.VehicleUVNTC",
	"VertexDecl.WaterSurface"
}};

class CModelData : public CPerm
{
public:
	CModelData()
	{
		m_HasTreeNodeItems = true;
	}

	void RenderTreeNode()
	{
		Illusion::ModelData_t* m_ModelData = reinterpret_cast<Illusion::ModelData_t*>(GetResourceData());
		if (!m_ModelData)
			return;

		Core_ImGui_ResourceHandleSelectable(u8"\u0103 Bone Palette", m_ModelData->m_BonePaletteHandle.m_NameUID, m_ModelData->m_NameUID);
		Core_ImGui_ResourceHandleSelectable(u8"\u0101 Material", m_ModelData->m_MaterialTableHandle.m_NameUID, m_ModelData->m_NameUID);
		Core_ImGui_TextSuffix("Num Prims", Format::Get("%u", m_ModelData->m_NumPrims));

		if (ImGui::TreeNodeEx(Format::Get(u8"\u0180 AABB##%u", m_ModelData->m_NameUID), IMGUI_TREENODE_FLAGS))
		{
			Core_ImGui_TextSuffix("Min", Format::Get("%.5f, %.5f, %.5f", m_ModelData->m_AABBMin[0], m_ModelData->m_AABBMin[1], m_ModelData->m_AABBMin[2]));
			Core_ImGui_TextSuffix("Max", Format::Get("%.5f, %.5f, %.5f", m_ModelData->m_AABBMax[0], m_ModelData->m_AABBMax[1], m_ModelData->m_AABBMax[2]));

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx(Format::Get(u8"\u0181 Meshes (Count: %u)##%u", m_ModelData->m_NumMeshes, m_ModelData->m_NameUID), IMGUI_TREENODE_FLAGS))
		{
			for (uint32_t i = 0; m_ModelData->m_NumMeshes > i; ++i)
			{
				Illusion::Mesh_t* m_Mesh = m_ModelData->GetMesh(i);
				if (ImGui::TreeNodeEx(Format::Get("%u##%u", i, m_ModelData->m_NameUID), IMGUI_TREENODE_FLAGS))
				{
					const char* m_VertexDecl = g_VertexDeclSymbolMap.Get(m_Mesh->m_VertexDeclHandle.m_NameUID);
					{
						if (m_VertexDecl)
							m_VertexDecl = &m_VertexDecl[11];
						else
							m_VertexDecl = Format::Get("0x%X", m_Mesh->m_VertexDeclHandle.m_NameUID);
					}
					Core_ImGui_TextSuffix("Vertex Declaration", m_VertexDecl);

					Core_ImGui_ResourceHandleSelectable("Material", m_Mesh->m_MaterialHandle.m_NameUID, m_ModelData->m_NameUID);
					Core_ImGui_ResourceHandleSelectable("Index", m_Mesh->m_IndexBufferHandle.m_NameUID, m_ModelData->m_NameUID);
					Core_ImGui_ResourceHandleSelectable("Vertex[0]", m_Mesh->m_VertexBufferHandles[0].m_NameUID, m_ModelData->m_NameUID);
					Core_ImGui_ResourceHandleSelectable("Vertex[1]", m_Mesh->m_VertexBufferHandles[1].m_NameUID, m_ModelData->m_NameUID);
					Core_ImGui_ResourceHandleSelectable("Vertex[2]", m_Mesh->m_VertexBufferHandles[2].m_NameUID, m_ModelData->m_NameUID);
					Core_ImGui_ResourceHandleSelectable("Vertex[3]", m_Mesh->m_VertexBufferHandles[3].m_NameUID, m_ModelData->m_NameUID);
					
					Core_ImGui_TextSuffix("Index Start", Format::Get("%u", m_Mesh->m_IndexStart));
					Core_ImGui_TextSuffix("Num Prims", Format::Get("%u", m_Mesh->m_NumPrims));

					const char* m_PrimType = "Unknown";
					switch (m_Mesh->m_PrimType)
					{
						case 0: m_PrimType = "Points"; break;
						case 1: m_PrimType = "Line-Strip"; break;
						case 2: m_PrimType = "Lines"; break;
						case 3: m_PrimType = "Triangles"; break;
						case 4: m_PrimType = "Triangle-Strip"; break;
						case 5: m_PrimType = "Triangle-Fan"; break;
						case 6: m_PrimType = "Quads"; break;
					}
					Core_ImGui_TextSuffix("Primitive Type", m_PrimType);

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void RenderProperties()
	{
		Illusion::ModelData_t* m_ModelData = reinterpret_cast<Illusion::ModelData_t*>(GetResourceData());
		if (!m_ModelData)
			return;

		if (ImGui::CollapsingHeader(u8"\u0180 AABB", IMGUI_TREENODE_OPEN_FLAGS))
		{
			ImGui::SetItemTooltip("Changing AABB does mostly affect the model culling boundary box.");
			ImGui::DragFloat3("Min", m_ModelData->m_AABBMin, 0.1f);
			ImGui::DragFloat3("Max", m_ModelData->m_AABBMax, 0.1f);

			ImGui::TreePop();
		}
	}
};