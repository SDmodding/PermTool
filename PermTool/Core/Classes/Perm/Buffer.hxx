#pragma once

class CBuffer : public CPerm
{
public:
	CBuffer()
	{
		m_HasTreeNodeItems = true;
	}

	void RenderTreeNode()
	{
		Illusion::Buffer_t* m_Buffer = reinterpret_cast<Illusion::Buffer_t*>(GetResourceData());

		Core_ImGui_TextSuffix("Size", Format::GetUInt(m_Buffer->m_Size));
		Core_ImGui_TextSuffix("Element Size", Format::GetUInt(m_Buffer->m_ElementSize));
		Core_ImGui_TextSuffix("Num Elements", Format::GetUInt(m_Buffer->m_NumElements));
	}
};