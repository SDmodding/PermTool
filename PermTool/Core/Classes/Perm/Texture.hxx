#pragma once

static std::pair<Illusion::eTextureFlags, const char*> g_FlagsList[] =
{
	{ Illusion::FLAG_CLAMPU,				"Clamp U" },
	{ Illusion::FLAG_CLAMPV,				"Clamp V" },
	{ Illusion::FLAG_MIRRORU,				"Mirror U" },
	{ Illusion::FLAG_MIRRORV,				"Mirror V" },
	{ Illusion::FLAG_LINEAR,				"Linear" },
	{ Illusion::FLAG_LE,					"LE" },
	{ Illusion::FLAG_CPU_WRITABLE,			"CPU Writable" },
	{ Illusion::FLAG_TARGET,				"Target" },
	{ Illusion::FLAG_PS3_TARGET_TILED,		"PS3 Target Tiled" },
	{ Illusion::FLAG_CREATED_AT_RUNTIME,	"Created at Run-time" },
	{ Illusion::FLAG_MOVING,				"Moving" },
	{ Illusion::FLAG_NO_EXPAND_AS_16,		"No Expand as 16" },
	{ Illusion::FLAG_MOVIE_TEXTURE,			"Movie Texture" },
	{ Illusion::FLAG_CPU_READABLE,			"CPU Readable" },
	{ Illusion::FLAG_MIPS_SHADER_SRC,		"Mips Shader Src" },
	{ Illusion::FLAG_PRESENT_BUFFER,		"Present Buffer" },
	{ Illusion::FLAG_ALIASED_TEXTURE,		"Aliased Texture" },
	{ Illusion::FLAG_PC_MAIN_MEM_COPY,		"PC Main Mem Copy" },
	{ Illusion::FLAG_PC_UNORDERED_ACCESS,	"PC Unordered Access" },
	{ Illusion::FLAG_REBIND_DATAHANDLE,		"Rebind Data Handle" },
	{ Illusion::FLAG_XB1_USE_ESRAM,			"XB1 Use ESRam" },
	{ Illusion::FLAG_MSAA_2X,				"MSAA 2X" },
	{ Illusion::FLAG_MSAA_4x,				"MSAA 4X" }
};

class CTexture : public CPerm
{
public:
	CTexture()
	{
		m_HasTreeNodeItems = true;
	}

	static const char* GetFilterName(Illusion::Texture_t* p_Texture)
	{
		switch (p_Texture->m_Filter)
		{
		case Illusion::FILTER_DEFAULT:
			return "Default";
		case Illusion::FILTER_LINEAR:
			return "Linear";
		case Illusion::FILTER_POINT:
			return "Point";
		case Illusion::FILTER_ANISOTROPIC:
			return "Anisotropic";
		case Illusion::FILTER_CONVOLUTION:
			return "Convolution";
		default:
			return "Unknown";
		}
	}

	static const char* GetAnisoName(Illusion::Texture_t* p_Texture)
	{
		switch (p_Texture->m_Aniso)
		{
		case Illusion::ANISO_X1:
			return "X1";
		case Illusion::ANISO_X2:
			return "X2";
		case Illusion::ANISO_X4:
			return "X4";
		case Illusion::ANISO_X6:
			return "X6";
		case Illusion::ANISO_X8:
			return "X8";
		case Illusion::ANISO_X10:
			return "X10";
		case Illusion::ANISO_X12:
			return "X12";
		case Illusion::ANISO_X16:
			return "X16";
		default:
			return "Unknown";
		}
	}

	static const char* GetTypeName(Illusion::Texture_t* p_Texture)
	{
		switch (p_Texture->m_Type)
		{
		case Illusion::TYPE_2D:
			return "2D";
		case Illusion::TYPE_3D:
			return "3D";
		case Illusion::TYPE_CUBE:
			return "Cube";
		case Illusion::TYPE_2D_ARRAY:
			return "2D (Array)";
		default:
			return "Unknown";
		}
	}

	static const char* GetFormatName(Illusion::Texture_t* p_Texture)
	{
		switch (p_Texture->m_Format)
		{
		case Illusion::FORMAT_A8R8G8B8:
			return "A8R8G8B8";
		case Illusion::FORMAT_DXT1:
			return "DXT1";
		case Illusion::FORMAT_DXT3:
			return "DXT3";
		case Illusion::FORMAT_DXT5:
			return "DXT5";
		case Illusion::FORMAT_R5G6B5:
			return "R5G6B5";
		case Illusion::FORMAT_A1R5G5B5:
			return "A1R5G5B";
		case Illusion::FORMAT_X8:
			return "X8";
		case Illusion::FORMAT_X16:
			return "X16";
		case Illusion::FORMAT_CXT1:
			return "CXT1";
		case Illusion::FORMAT_DXN:
			return "DXN";
		case Illusion::FORMAT_BC6H_UF16:
			return "BC6H_UF16";
		case Illusion::FORMAT_BC6H_SF16:
			return "BC6H_SF16";
		case Illusion::FORMAT_BC7_UNORM:
			return "BC7_UNORM";
		case Illusion::FORMAT_BC7_UNORM_SRGB:
			return "BC7_UNORM_SRGB";
		case Illusion::FORMAT_R32F:
			return "R32F";
		case Illusion::FORMAT_X16FY16FZ16FW16F:
			return "X16FY16FZ16FW16F";
		case Illusion::FORMAT_D24S8:
			return "D24S8";
		case Illusion::FORMAT_D24FS8:
			return "D24FS8";
		case Illusion::FORMAT_SHADOW:
			return "Shadow";
		case Illusion::FORMAT_DEPTHCOPY:
			return "Depth Copy";
		case Illusion::FORMAT_A2R10G10B10:
			return "A2R10G10B10";
		case Illusion::FORMAT_A2R10G10B10F:
			return "A2R10G10B10F";
		case Illusion::FORMAT_A16B16G16R16:
			return "A16B16G16R16";
		default:
			return "Unknown";
		}
	}

	static const char* GetMipmapBiasPresetName(Illusion::Texture_t* p_Texture)
	{
		switch (p_Texture->m_MipMapBiasPreset)
		{
		case Illusion::MIPMAP_BIAS_PRESET_UNSPECIFIED:
			return "Unspecified";
		case Illusion::MIPMAP_BIAS_PRESET_EAGLE_EYE:
			return "Eagle Eye";
		case Illusion::MIPMAP_BIAS_PRESET_SLIDER0:
			return "Slider (0)";
		case Illusion::MIPMAP_BIAS_PRESET_SLIDER1:
			return "Slider (1)";
		case Illusion::MIPMAP_BIAS_PRESET_SLIDER2:
			return "Slider (2)";
		case Illusion::MIPMAP_BIAS_PRESET_SLIDER3:
			return "Slider (3)";
		case Illusion::MIPMAP_BIAS_PRESET_SLIDER4:
			return "Slider (4)";
		case Illusion::MIPMAP_BIAS_PRESET_CUSTOM:
			return "Custom";
		default:
			return "Unknown";
		}
	}

	void RenderTreeNode()
	{
		Illusion::Texture_t* m_Texture = reinterpret_cast<Illusion::Texture_t*>(GetResourceData());

		static constexpr float m_SuffixStartX = 180.f;
		Core_ImGui_TextSuffix("Width", Format::GetUInt16(m_Texture->m_Width), m_SuffixStartX);
		Core_ImGui_TextSuffix("Height", Format::GetUInt16(m_Texture->m_Height), m_SuffixStartX);
		Core_ImGui_TextSuffix("AlphaState", g_AlphaStateSymbolMap.Get(m_Texture->m_AlphaStateUID, Format::GetUIntHex(m_Texture->m_AlphaStateUID)), m_SuffixStartX);
		Core_ImGui_TextSuffix("Aniso", GetAnisoName(m_Texture), m_SuffixStartX);
		Core_ImGui_TextSuffix("Filter", GetFilterName(m_Texture), m_SuffixStartX);
		Core_ImGui_TextSuffix("Format", GetFormatName(m_Texture), m_SuffixStartX);
		Core_ImGui_TextSuffix("Type", GetTypeName(m_Texture), m_SuffixStartX);
		Core_ImGui_TextSuffix("Num Mipmaps", Format::GetUInt16(static_cast<uint16_t>(m_Texture->m_NumMipMaps)), m_SuffixStartX);
		Core_ImGui_TextSuffix("Mipmap Bias Preset", GetMipmapBiasPresetName(m_Texture), m_SuffixStartX);

		if (ImGui::TreeNodeEx(u8"\uF024 Flags", IMGUI_TREENODE_FLAGS))
		{
			for (auto& m_Pair : g_FlagsList)
			{
				if (m_Texture->m_Flags & m_Pair.first)
					ImGui::BulletText(m_Pair.second);
			}

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNodeEx(u8"\uF0C7 Data Info (Temp File)", IMGUI_TREENODE_FLAGS))
		{
			Core_ImGui_TextSuffix("Size", Format::Get("%u (0x%X)", m_Texture->m_ImageDataByteSize, m_Texture->m_ImageDataByteSize), m_SuffixStartX);
			Core_ImGui_TextSuffix("File Offset", Format::Get("%llu (0x%lX)", m_Texture->m_ImageDataPosition, m_Texture->m_ImageDataPosition), m_SuffixStartX);

			if (ImGui::Button("Copy to clipboard", { -1.f, 0.f }))
				ImGui::SetClipboardText(Format::Get("Size: %u (0x%X)\nFile Offset: %llu (0x%lX)",
					m_Texture->m_ImageDataByteSize, m_Texture->m_ImageDataByteSize,
					m_Texture->m_ImageDataPosition, m_Texture->m_ImageDataPosition));

			ImGui::TreePop();
		}
	}

	void RenderProperties()
	{
		Illusion::Texture_t* m_Texture = reinterpret_cast<Illusion::Texture_t*>(GetResourceData());
		
		if (ImGui::CollapsingHeader(u8"\uF013 Configuration", IMGUI_TREENODE_OPEN_FLAGS))
		{
			ImGui::PushItemWidth(-90.f);

			int m_NumMipmaps = static_cast<int>(m_Texture->m_NumMipMaps);
			int m_MaxMipmaps = 1;
			{
				uint16_t m_XYSize = min(m_Texture->m_Width, m_Texture->m_Height);
				while (m_XYSize > 4)
				{
					++m_MaxMipmaps;
					m_XYSize /= 2;
				}
			}
			if (ImGui::SliderInt("Num Mipmaps", &m_NumMipmaps, 0, m_MaxMipmaps, "%d", ImGuiSliderFlags_AlwaysClamp))
				m_Texture->m_NumMipMaps = static_cast<uint8_t>(m_NumMipmaps);

			const char* m_AlphaStateName = g_AlphaStateSymbolMap.Get(m_Texture->m_AlphaStateUID);
			if (ImGui::BeginCombo("AlphaState", (m_AlphaStateName ? m_AlphaStateName : Format::GetUIntHex(m_Texture->m_AlphaStateUID))))
			{
				auto m_Items = g_AlphaStateSymbolMap.GetSortedVector();
				if (!m_AlphaStateName)
					m_Items.insert(m_Items.begin(), { m_Texture->m_AlphaStateUID, Format::GetUIntHex(m_Texture->m_AlphaStateUID) });

				for (auto& m_Pair : m_Items)
				{
					bool m_Selected = (m_Pair.first == m_Texture->m_AlphaStateUID);
					if (ImGui::Selectable(m_Pair.second, &m_Selected))
						m_Texture->m_AlphaStateUID = m_Pair.first;
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
		}

		if (ImGui::CollapsingHeader(u8"\uF024 Flags", IMGUI_TREENODE_OPEN_FLAGS))
		{
			if (ImGui::BeginListBox("##TextureFlags", { -1.f, 0.f }))
			{
				for (auto& m_Pair : g_FlagsList)
				{
					bool m_Selected = (m_Texture->m_Flags & m_Pair.first);
					if (ImGui::Selectable(m_Pair.second, m_Selected))
					{
						if (m_Selected)
							m_Texture->m_Flags = static_cast<Illusion::eTextureFlags>(m_Texture->m_Flags & ~m_Pair.first);
						else
							m_Texture->m_Flags = static_cast<Illusion::eTextureFlags>(m_Texture->m_Flags | m_Pair.first);
					}
				}

				ImGui::EndListBox();
			}
		}
	}
};