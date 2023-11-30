#pragma once

namespace Format
{
	static char g_Buffer[1024];

	const char* Get(const char* p_Format, ...)
	{
		va_list m_Args;
		va_start(m_Args, p_Format);
		vsnprintf(g_Buffer, sizeof(g_Buffer), p_Format, m_Args);
		va_end(m_Args);

		return g_Buffer;
	}

	__inline const char* GetUInt16(uint16_t p_Value)
	{
		return Get("%hu", p_Value);
	}

	__inline const char* GetInt16(int16_t p_Value)
	{
		return Get("%hd", p_Value);
	}

	__inline const char* GetInt(int p_Value)
	{
		return Get("%d", p_Value);
	}

	__inline const char* GetUInt(uint32_t p_Value)
	{
		return Get("%u", p_Value);
	}

	__inline const char* GetUIntHex(uint32_t p_Value)
	{
		return Get("0x%X", p_Value);
	}

}