#pragma once

class CArgs
{
public:
	char** m_Table = __argv;
	int m_Count = __argc;

	int GetIndex(const char* p_Arg)
	{
		for (int i = 0; m_Count > i; ++i)
		{
			char* m_Arg = m_Table[i];
			if (m_Arg[0] == '\0' || m_Arg[1] == '\0')
				continue;

			if (_stricmp(p_Arg, &m_Arg[1]) == 0)
				return i;
		}

		return -1;
	}

	__inline bool IsSet(const char* p_Arg)
	{
		return (GetIndex(p_Arg) >= 0);
	}

	__inline char* GetValue(int p_Index)
	{
		int m_Index = (p_Index + 1);
		if (m_Index == 0 || m_Index >= m_Count)
			return nullptr;

		return m_Table[m_Index];
	}

	__inline char* GetValue(const char* p_Arg)
	{
		return GetValue(GetIndex(p_Arg));
	}
};
CArgs g_Args;