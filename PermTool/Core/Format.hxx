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
}