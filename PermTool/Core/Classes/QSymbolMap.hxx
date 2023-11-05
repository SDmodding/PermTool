#pragma once

class CQSymbolMap
{
public:
	std::unordered_map<uint32_t, const char*> m_Map;

	CQSymbolMap() { }
	CQSymbolMap(std::initializer_list<const char*> p_List, bool p_Uppercase = false)
	{
		auto m_Strings = p_List.begin();
		size_t m_Size = p_List.size();
		for (size_t i = 0; m_Size > i; ++i)
		{
			uint32_t m_Hash = (p_Uppercase ? SDK::StringHashUpper32(m_Strings[i]) : SDK::StringHash32(m_Strings[i]));
			m_Map[m_Hash] = m_Strings[i];
		}
	}

	const char* Get(uint32_t p_Symbol, const char* p_Default = nullptr)
	{
		auto m_Find = m_Map.find(p_Symbol);
		if (m_Find != m_Map.end())
			return (*m_Find).second;

		return p_Default;
	}
};