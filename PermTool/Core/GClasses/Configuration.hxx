#pragma once
#define CONFIGURATION_FILE		"./Configuration.ini"

class CConfiguration
{
public:
	struct Warning_t
	{
		bool m_MoveResource = false;
		bool m_MoveResourceShown = false;

		bool CanShowMoveResource()
		{
			if (m_MoveResource || m_MoveResourceShown)
				return false;

			m_MoveResourceShown = true;
			return true;
		}
	};
	Warning_t Warning;

	CConfiguration()
	{
		Warning.m_MoveResource = GetPrivateProfileIntA("Warning", "MoveResource", 0, CONFIGURATION_FILE);
	}

	void WriteBoolean(const char* p_Section, const char* p_Key, bool p_Value)
	{
		char m_Str[2] = { 
			(p_Value ? '1' : '0'),
			'\0' 
		};

		WritePrivateProfileStringA(p_Section, p_Key, m_Str, CONFIGURATION_FILE);
	}
};
static CConfiguration g_Configuration;