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
		if (!m_Buffer)
			return;

		Core_ImGui_TextSuffix("Size", Format::Get("%u", m_Buffer->m_Size));
		Core_ImGui_TextSuffix("Element Size", Format::Get("%u", m_Buffer->m_ElementSize));
		Core_ImGui_TextSuffix("Num Elements", Format::Get("%u", m_Buffer->m_NumElements));
	}
};