#pragma once

namespace Helper
{
	namespace UIScreen
	{
		enum eScribeRes
		{
			eScribeRes_OK = 0,
			eScribeRes_IOError,
			eScribeRes_InvalidFileHeader,
			eScribeRes_InvalidFlashSignature,
			eScribeRes_InvalidFlashVersion
		};
		const char* GetResultStr(eScribeRes p_Res)
		{
			switch (p_Res)
			{
			case eScribeRes_IOError:
				return "Couldn't open/read file.";
			case eScribeRes_InvalidFileHeader:
				return "File has invalid header.";
			case eScribeRes_InvalidFlashSignature:
				return "File has invalid flash header.";
			case eScribeRes_InvalidFlashVersion:
				return "File has invalid flash version (Must be 8).";
			default:
				return nullptr;
			}
		}

		namespace Internal
		{
			eScribeRes Scribe(UFG::UIScreen_t** p_Result, const char* p_SWFPath, const char* p_Name)
			{
				FILE* m_File = fopen(p_SWFPath, "rb");
				if (!m_File)
					return eScribeRes_IOError;

				fseek(m_File, 0, SEEK_END);

				size_t m_FileSize = static_cast<size_t>(ftell(m_File));
				if (0x8 > m_FileSize)
				{
					fclose(m_File);
					return eScribeRes_InvalidFileHeader;
				}

				uint8_t* m_FileData = new uint8_t[m_FileSize];

				fseek(m_File, 0, SEEK_SET);

				size_t m_ReadRes = fread(m_FileData, 1, m_FileSize, m_File);

				fclose(m_File);

				if (m_ReadRes != m_FileSize)
				{
					delete[] m_FileData;
					return eScribeRes_IOError;
				}

				if (!((m_FileData[0] == 'S' && m_FileData[1] == 'F' && m_FileData[2] == 'X') || // SWF
					  (m_FileData[0] == 'C' && m_FileData[1] == 'F' && m_FileData[2] == 'X') || // CFX
					  (m_FileData[0] == 'C' && m_FileData[1] == 'W' && m_FileData[2] == 'S') || // CWS
					  (m_FileData[0] == 'G' && m_FileData[1] == 'F' && m_FileData[2] == 'X')))  // GFX
				{
					delete[] m_FileData;
					return eScribeRes_InvalidFlashSignature;
				}

				if (m_FileData[3] != 0x8)
				{
					delete[] m_FileData;
					return eScribeRes_InvalidFlashVersion;
				}

				size_t m_DataSize = Perm::GetAlignSize(sizeof(UFG::UIScreen_t) + m_FileSize);
				UFG::UIScreen_t* m_UIScreen = reinterpret_cast<UFG::UIScreen_t*>(malloc(m_DataSize));
				ZeroMemory(m_UIScreen, m_DataSize);
				{
					m_UIScreen->m_TypeUID = m_UIScreen->m_ChunkUID = 0x442A39D9;
					m_UIScreen->SetEntrySize(static_cast<uint32_t>(m_DataSize));

					m_UIScreen->m_NameUID = SDK::StringHashUpper32(p_Name);
					strncpy(m_UIScreen->m_DebugName, p_Name, sizeof(UFG::UIScreen_t::m_DebugName));

					m_UIScreen->m_BufferSize = static_cast<uint32_t>(m_FileSize);
					m_UIScreen->m_BufferDataOffset = 0x8;

					memcpy(m_UIScreen->GetBufferData(), m_FileData, m_FileSize);
					delete[] m_FileData;
				}

				*p_Result = m_UIScreen;
				return eScribeRes_OK;
			}
		}

		void Scribe()
		{
			OPENFILENAMEA m_OpenFileName = { 0 };
			{
				ZeroMemory(&m_OpenFileName, sizeof(OPENFILENAMEA));
				m_OpenFileName.lStructSize = sizeof(OPENFILENAMEA);
			}

			char m_FilePathBuffer[MAX_PATH] = { '\0' };
			m_OpenFileName.lpstrFile = m_FilePathBuffer;
			m_OpenFileName.nMaxFile = sizeof(m_FilePathBuffer);
			m_OpenFileName.lpstrFilter = "(SWF File)\0*.swf\0";
			m_OpenFileName.Flags = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
			m_OpenFileName.hwndOwner = g_Window;
			m_OpenFileName.lpstrTitle = "Select SWF File";
			if (GetOpenFileNameA(&m_OpenFileName) == 0)
				return;

			std::filesystem::path m_FilePath = m_FilePathBuffer;

			UFG::UIScreen_t* m_UIScreen = nullptr;
			std::string m_UIScreenName = m_FilePath.stem().string();
			eScribeRes m_Res = Internal::Scribe(&m_UIScreen, m_FilePathBuffer, m_UIScreenName.c_str());
			if (m_Res != eScribeRes_OK)
			{
				g_PopupHandler.AddText(g_PopupErrorTitle, GetResultStr(m_Res));
				return;
			}

			std::string m_ScribePath = m_FilePath.remove_filename().string() + m_UIScreenName + ".bin";
			FILE* m_File = fopen(m_ScribePath.c_str(), "wb");
			if (m_File)
			{
				fwrite(m_UIScreen, static_cast<size_t>(m_UIScreen->GetEntrySize()), 1, m_File);
				fclose(m_File);
			}
			else
				g_PopupHandler.AddText(g_PopupErrorTitle, "Couldn't open file for writing.");
			
			free(m_UIScreen);
		}
	}
}