#pragma once

class CPopupHandler
{
public:
	struct Popup_t
	{
		std::string m_Title;
		std::string m_Text;
		ImVec2 m_Size;
		void* m_RenderCallback = nullptr;
		void* m_RenderCallbackData = nullptr;
	};
	std::vector<Popup_t> m_List;

	__inline void Push(Popup_t& p_Popup) { m_List.emplace_back(p_Popup); }

	__inline void AddCallback(const char* p_Title, const ImVec2& p_Size, void* p_RenderCallback, void* p_RenderCallbackData = nullptr)
	{
		Popup_t m_Popup;
		m_Popup.m_Title = p_Title;
		m_Popup.m_Size = p_Size;
		m_Popup.m_RenderCallback = p_RenderCallback;
		m_Popup.m_RenderCallbackData = p_RenderCallbackData;
		Push(m_Popup);
	}

	__inline void AddText(const char* p_Title, const char* p_Text)
	{
		Popup_t m_Popup;
		m_Popup.m_Title = p_Title;
		m_Popup.m_Text = p_Text;
		m_Popup.m_Size = { 300.f, -1.f };
		Push(m_Popup);
	}

	void Render()
	{
		if (m_List.empty())
			return;

		Popup_t& m_Popup = m_List[0];
		if (!ImGui::IsPopupOpen(m_Popup.m_Title.c_str()))
			ImGui::OpenPopup(m_Popup.m_Title.c_str());

		ImGui::SetNextWindowSize(m_Popup.m_Size, ImGuiCond_Always);
		if (ImGui::BeginPopupModal(m_Popup.m_Title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			if (m_Popup.m_RenderCallback)
			{
				if (reinterpret_cast<bool(*)(void*)>(m_Popup.m_RenderCallback)(m_Popup.m_RenderCallbackData))
				{
					m_List.erase(m_List.begin());
					ImGui::CloseCurrentPopup();
				}
			}
			else
			{
				ImGui::TextWrapped(m_Popup.m_Text.c_str());

				if (ImGui::Button("OK", { -1.f, 0.f }))
				{
					m_List.erase(m_List.begin());
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
};
CPopupHandler g_PopupHandler;