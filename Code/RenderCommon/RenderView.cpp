#include "StdAfx.h"
#include "RenderView.h"

RenderView::RenderView()
{
}

RenderView::~RenderView()
{
}

void RenderView::SetFrameId(int frameId)
{
}

int RenderView::GetFrameId() const
{
	return 0;
}

void RenderView::SetCameras(const Camera* pCameras, int camerasCount)
{
}

void RenderView::SetTargetClearColor(const Color& color)
{
}

int RenderView::GetLightsCount() const
{
	return 0;
}

void RenderView::Submit(RenderItem item, RenderListId listId)
{
	auto id = static_cast<uint8>(listId);
	m_renderLists[id].push_back(item);
}

void RenderView::ExecuteRenderPass(RenderListId listId)
{
	assert(m_pDeviceContext);

	switch (listId)
	{
	case RenderListId::ShadowGen:
		Execute_ShadowPass();
		break;
	case RenderListId::ZPrePass:
		Execute_EarlyZPass();
		break;
	case RenderListId::LightPass:
		Execute_LightsPass();
		break;
	case RenderListId::Opaque:
		Execute_OpaquePass();
		break;
	case RenderListId::Transparent:
		Execute_TranspPass();
		break;
	case RenderListId::PostEffects:
		Execute_PostEffect();
		break;
	default:
		assert(false);
		break;
	}
}

bool RenderView::HasAddedItems(RenderListId listId)
{
	auto id = static_cast<uint8>(listId);
	return m_renderLists[id].size() > 0;
}

size_t RenderView::NumAddedItems(RenderListId listId)
{
	auto id = static_cast<uint8>(listId);
	return m_renderLists[id].size();
}

void RenderView::Execute_ShadowPass()
{
	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::ShadowGen)])
	{
		VertexFormat		vtxFmt			= renderItem.pRenderMesh->GetVertexFormat();
		ShaderTechnique*	pTech			= renderItem.pShaderTechnique;
		GpuBuffer*			pVertexBuffer	= renderItem.pRenderMesh->GetVertexBuffer();
		GpuBuffer*			pIndexBuffer	= renderItem.pRenderMesh->GetIndexBuffer();
		IGpuBuffer*			pVBuffer		= pVertexBuffer->GetDevBuffer()->GetBuffer();
		IGpuBuffer*			pIBuffer		= pIndexBuffer->GetDevBuffer()->GetBuffer();
		IVertexLayout*		pCurLayout		= nullptr;
		// Set constant buffer per draw (if needed for shadow pass)

		m_pDeviceContext->IASetVertexBuffers(0, 1, &pVBuffer, , );
		m_pDeviceContext->IASetIndexBuffer(pIBuffer, , );

		// TODO: DON'T FORGET TO SET WORLD VIEW PROJECT MATRIX OF A LIGHT SOURCE (IN THIS CASE THE SUN).

		m_pDeviceContext->VSSetConstantBuffers(BufferSlot::PerDraw, 1, &pConstBuffers);
		m_pDeviceContext->PSSetConstantBuffers(BufferSlot::PerDraw, 1, &pConstBuffers);

		for (auto& pass : pTech->m_Passes)
		{
			Shader* pVS = pass.m_pVertexShader;
			Shader* pPS = pass.m_pPixelShader;

			IVertexLayout* pNewLayout = DeviceFactory::Get().GetOrCreateInputLayout(pVS->GetShaderBlob(), vtxFmt)->second;
			if (pNewLayout != pCurLayout)
			{
				pCurLayout = pNewLayout;
				m_pDeviceContext->IASetInputLayout(pCurLayout);
				m_pDeviceContext->IASetPrimitiveTopology();
			}

			m_pDeviceContext->VSSetShader(pVS, nullptr, 0);
			m_pDeviceContext->PSSetShader(pPS, nullptr, 0);

			// Set samplers from pass
			// Set shader resources
		}
	}

	GpuDSV* pShadowMap = RenderResources::s_pTexShadowMap->GetDeviceTexture()->LookupDSV(ResourceViewType::DepthOnly);
	m_pDeviceContext->OMSetRenderTargets(0, nullptr, pShadowMap);
}

void RenderView::Execute_EarlyZPass()
{
	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::ZPrePass)])
	{

	}
}

void RenderView::Execute_LightsPass()
{
	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::LightPass)])
	{

	}
}

void RenderView::Execute_OpaquePass()
{
	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::Opaque)])
	{

	}
}

void RenderView::Execute_TranspPass()
{
	//for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::Transparent)])
	//{
		assert(false); // TODO: Not implemented
	//}
}

void RenderView::Execute_PostEffect()
{
	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::PostEffects)])
	{
		//gRenderer->PostEffectsMng()->Exec();
	}
}
