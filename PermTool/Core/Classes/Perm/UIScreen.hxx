#pragma once

class CUIScreen : public CPerm
{
public:
	CUIScreen()
	{
		m_HasCtxOptions = true;
	}

	void ExportSWF()
	{
		OPENFILENAMEA m_OpenFileName = { 0 };
		{
			ZeroMemory(&m_OpenFileName, sizeof(OPENFILENAMEA));
			m_OpenFileName.lStructSize = sizeof(OPENFILENAMEA);
		}

		char m_FilePath[MAX_PATH] = { '\0' };
		m_OpenFileName.lpstrFile = m_FilePath;
		m_OpenFileName.nMaxFile = sizeof(m_FilePath);
		m_OpenFileName.lpstrFilter = "(SWF File)\0*.swf\0";
		m_OpenFileName.Flags = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
		m_OpenFileName.hwndOwner = g_Window;
		m_OpenFileName.lpstrTitle = "Export SWF File";
		if (GetSaveFileNameA(&m_OpenFileName) == 0)
			return;

		std::string m_SWFPath = m_FilePath;
		if (m_SWFPath.find(".swf") == std::string::npos)
			m_SWFPath += ".swf";

		FILE* m_File = fopen(&m_SWFPath[0], "wb");
		if (!m_File)
			return;

		UFG::UIScreen_t* m_UIScreen = reinterpret_cast<UFG::UIScreen_t*>(GetResourceData());
		fwrite(m_UIScreen->GetBufferData(), 1, static_cast<size_t>(m_UIScreen->m_BufferSize), m_File);
		fclose(m_File);
	}

	void RenderCtxOptions()
	{
		if (ImGui::MenuItem(u8"\uF56E Export as SWF File"))
			ExportSWF();
	}
};