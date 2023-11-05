//=============================================================================
//
// Description:		Main include file for perm classes handling and other
//					related stuff around perm...
// 
//=============================================================================
#pragma once

class CPerm
{
public:
	void* m_DataPtr = nullptr;
	size_t m_DataSize = 0;

	bool m_HasTreeNodeItems = false;

	virtual ~CPerm() 
	{
		if (m_DataPtr)
		{
			free(m_DataPtr);
			m_DataPtr = nullptr;
		}
	}

	CPerm() { }

	bool InitializeData(void* p_Ptr, size_t p_Size)
	{
		m_DataPtr = malloc(p_Size);
		if (!m_DataPtr)
			return false;

		m_DataSize = p_Size;
		memcpy(m_DataPtr, p_Ptr, p_Size);
		return true;
	}

	// Resource Functions

	__inline UFG::ResourceEntry_t* GetResourceEntry() { return reinterpret_cast<UFG::ResourceEntry_t*>(m_DataPtr); }

	CResourceData* GetResourceData()
	{
		UFG::ResourceEntry_t* m_ResourceEntry = GetResourceEntry();
		if (m_ResourceEntry)
			return reinterpret_cast<CResourceData*>(m_ResourceEntry->GetData());

		return nullptr;
	}

	// Virtual Functions

	virtual void OnDataLoad() { }

	virtual void RenderTreeNode() { }
	virtual void RenderProperties() { }
};

// Perm Types
#include "BonePalette.hxx"
#include "Buffer.hxx"
#include "Material.hxx"
#include "ModelData.hxx"
#include "UILocalization.hxx"

// Functions
namespace Perm
{
	CPerm* Get(uint32_t p_TypeUID)
	{
		switch (p_TypeUID)
		{
		case RESOURCE_TYPE_BonePalette:
			return new CBonePalette;
		case RESOURCE_TYPE_Buffer:
			return new CBuffer;
		case RESOURCE_TYPE_Material:
			return new CMaterial;
		case RESOURCE_TYPE_ModelData:
			return new CModelData;
		case RESOURCE_TYPE_UILocalization:
			return new CUILocalization;
		default:
			return new CPerm;
		}
	}
}