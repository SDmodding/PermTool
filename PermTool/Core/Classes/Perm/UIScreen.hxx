#pragma once

class CUIScreen : public CPerm
{
public:
	CUIScreen()
	{
		m_HasCtxOptions = true;
	}

	bool OutputSWF(const char* p_FilePath)
	{
		FILE* m_File = fopen(p_FilePath, "wb");
		if (!m_File)
			return false;

		UFG::UIScreen_t* m_UIScreen = reinterpret_cast<UFG::UIScreen_t*>(GetResourceData());
		fwrite(m_UIScreen->GetBufferData(), 1, static_cast<size_t>(m_UIScreen->m_BufferSize), m_File);
		fclose(m_File);
		return true;
	}

	void ExportSWF()
	{
		UFG::UIScreen_t* m_UIScreen = reinterpret_cast<UFG::UIScreen_t*>(GetResourceData());

		char m_FilePath[MAX_PATH]; 
		{
			ZeroMemory(m_FilePath, sizeof(m_FilePath));
			strncpy_s(m_FilePath, this->GetResoruceName(), sizeof(m_FilePath));
		}

		OPENFILENAMEA m_OpenFileName = { 0 };
		{
			ZeroMemory(&m_OpenFileName, sizeof(OPENFILENAMEA));
			m_OpenFileName.lStructSize = sizeof(OPENFILENAMEA);
			m_OpenFileName.lpstrFile = m_FilePath;
			m_OpenFileName.nMaxFile = sizeof(m_FilePath);
			m_OpenFileName.lpstrFilter = "(SWF File)\0*.swf\0";
			m_OpenFileName.Flags = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
			m_OpenFileName.hwndOwner = g_Window;
			m_OpenFileName.lpstrTitle = "Export SWF File";
		}
		if (GetSaveFileNameA(&m_OpenFileName) == 0)
			return;

		std::string m_SWFPath = m_FilePath;
		if (m_SWFPath.find(".swf") == std::string::npos)
			m_SWFPath += ".swf";

		OutputSWF(&m_SWFPath[0]);
	}

	void RenderCtxOptions()
	{
		if (ImGui::MenuItem(u8"\uF56E Export as SWF File"))
			ExportSWF();
	}
};