#include "StdAfx.h"
#include "DeviceObjects.h"

IVertexLayout* DeviceFactory::CreateInputLayout(const InputLayout& layout, const ShaderBlob* pVertexShader)
{
	if (!pVertexShader || !pVertexShader->m_pData)
		return nullptr;

	const int   size	= pVertexShader->m_size;
	const void* pData	= pVertexShader->m_pData;

	IVertexLayout* pLayout = nullptr;
	if (FAILED( DeviceFactory::Get().m_pDevice->CreateInputLayout(&layout.m_ElementsDesc[0], layout.m_ElementsDesc.size(), pData, size, &pLayout) ))
	{
		assert(false);
		return pLayout;
	}

	return pLayout;
}