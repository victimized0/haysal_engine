#include "StdAfx.h"
#include "RenderView.h"

RenderView::RenderView()
{

	m_pcbPerDraw		= gRenderer->GetBufMan()->CreateConstBuffer(sizeof(CB_PerDraw));
	m_pcbPerMaterial	= gRenderer->GetBufMan()->CreateConstBuffer(sizeof(CB_PerMaterial));
	m_pcbPerPass		= gRenderer->GetBufMan()->CreateConstBuffer(sizeof(CB_PerPass));
	m_pcbPerFrame		= gRenderer->GetBufMan()->CreateConstBuffer(sizeof(CB_PerFrame));
	m_pcbPerShadowPass	= gRenderer->GetBufMan()->CreateConstBuffer(sizeof(CB_PerShadowPass));

	m_pcbPerDraw		->SetDebugName("PER_DRAW");
	m_pcbPerMaterial	->SetDebugName("PER_MATERIAL");
	m_pcbPerPass		->SetDebugName("PER_PASS");
	m_pcbPerFrame		->SetDebugName("PER_FRAME");
	m_pcbPerShadowPass	->SetDebugName("PER_SHADOW_PASS");
}

RenderView::~RenderView()
{
	SAFE_DELETE(m_pcbPerDraw);
	SAFE_DELETE(m_pcbPerMaterial);
	SAFE_DELETE(m_pcbPerPass);
	SAFE_DELETE(m_pcbPerFrame);
	SAFE_DELETE(m_pcbPerShadowPass);
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
	uint8 id = static_cast<uint8>(listId);
	m_renderLists[id].push_back(item);
}

void RenderView::ExecuteRenderPass(RenderListId listId)
{
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
		//Execute_TranspPass();
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

void RenderView::SetFrameData(void* pData, size_t size)
{
	m_pcbPerFrame->Update(pData, size);

	IGpuBuffer* ppCBPerFrame = m_pcbPerFrame->GetBuffer();
	DeviceFactory::Get().GetContext()->VSSetConstantBuffers( CB_PerFrame::Slot, 1, &ppCBPerFrame );
	DeviceFactory::Get().GetContext()->PSSetConstantBuffers( CB_PerFrame::Slot, 1, &ppCBPerFrame );
}

void RenderView::Execute_ShadowPass()
{
	GpuContext* pContext = DeviceFactory::Get().GetContext();
	gRenderer->PushProfileMarker("SHADOW_PASS");

	CB_PerDraw cbpd;
	CB_PerShadowPass cpbsp;

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

		//cpbsp.LightPos	= sunDir;
		//cpbsp.ViewPos	= camPos;
		m_pcbPerShadowPass->Update(reinterpret_cast<void*>(&cpbsp), sizeof(CB_PerShadowPass));

		IGpuBuffer* ppCBPerShadowPass = m_pcbPerShadowPass->GetBuffer();
		pContext->VSSetConstantBuffers( CB_PerShadowPass::Slot, 1, &ppCBPerShadowPass );

		pContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(renderItem.pRenderMesh->GetPrimitiveTopology()));

		const UINT stride = VertexFormatsHelper::GetStride(vtxFmt);
		pContext->IASetVertexBuffers(0, 1, &pVBuffer, &stride, 0);
		pContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
		// TODO: DON'T FORGET TO SET WORLD VIEW PROJECT MATRIX OF A LIGHT SOURCE (IN THIS CASE THE SUN).

		cbpd.WorldMatrix = renderItem.WorldMatrix;
		m_pcbPerDraw->Update(reinterpret_cast<void*>(&cbpd), sizeof(CB_PerDraw));

		IGpuBuffer* ppCBPerDraw = m_pcbPerDraw->GetBuffer();
		pContext->VSSetConstantBuffers( CB_PerDraw::Slot, 1, &ppCBPerDraw );
		//pContext->PSSetConstantBuffers( CB_PerDraw::Slot, 1, 1, &ppCBPerDraw );

		GpuDSV* pShadowMap = RenderResources::s_pTexShadowMap->GetDeviceTexture()->LookupDSV(ResourceViewType::DepthOnly);
		pContext->OMSetRenderTargets(0, nullptr, pShadowMap);

		for (auto& pass : pTech->m_Passes)
		{
			Shader* pVS = pass.m_pVertexShader;
			//Shader* pPS = pass.m_pPixelShader;

			IVertexLayout* pNewLayout = DeviceFactory::Get().GetOrCreateInputLayout(pVS->GetShaderBlob(), vtxFmt)->second;
			if (pNewLayout != pCurLayout)
			{
				pCurLayout = pNewLayout;
				pContext->IASetInputLayout(pCurLayout);
			}

			//pContext->VSSetShader(pVS., nullptr, 0);
			//pContext->PSSetShader(pPS, nullptr, 0);

			// Set samplers from pass
			// Set shader resources

			pContext->DrawIndexed(renderItem.pRenderMesh->GetIndicesCount(), 0, 0);
		}
	}

	gRenderer->PopProfileMarker("SHADOW_PASS");
}

void RenderView::Execute_EarlyZPass()
{
	gRenderer->PushProfileMarker("DEPTH_PREPASS");

	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::ZPrePass)])
	{

	}

	gRenderer->PopProfileMarker("DEPTH_PREPASS");
}

void RenderView::Execute_LightsPass()
{
	gRenderer->PushProfileMarker("LIGHT_PASS");

	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::LightPass)])
	{

	}

	gRenderer->PopProfileMarker("LIGHT_PASS");
}

void RenderView::Execute_OpaquePass()
{
	gRenderer->PushProfileMarker("OPAQUE_PASS");

	GpuContext* pContext = DeviceFactory::Get().GetContext();
	gRenderer->PushProfileMarker("SHADOW_PASS");

	CB_PerDraw cbpd;

	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::Opaque)])
	{
		VertexFormat		vtxFmt			= renderItem.pRenderMesh->GetVertexFormat();
		ShaderTechnique*	pTech			= renderItem.pShaderTechnique;
		GpuBuffer*			pVertexBuffer	= renderItem.pRenderMesh->GetVertexBuffer();
		GpuBuffer*			pIndexBuffer	= renderItem.pRenderMesh->GetIndexBuffer();
		IGpuBuffer*			pVBuffer		= pVertexBuffer->GetDevBuffer()->GetBuffer();
		IGpuBuffer*			pIBuffer		= pIndexBuffer->GetDevBuffer()->GetBuffer();
		IVertexLayout*		pCurLayout		= nullptr;

		pContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(renderItem.pRenderMesh->GetPrimitiveTopology()));

		const UINT stride = VertexFormatsHelper::GetStride(vtxFmt);
		const UINT ofsets = 0;
		pContext->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &ofsets);
		pContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

		cbpd.WorldMatrix = renderItem.WorldMatrix;
		m_pcbPerDraw->Update(reinterpret_cast<void*>(&cbpd), sizeof(CB_PerDraw));

		IGpuBuffer* ppCBPerDraw = m_pcbPerDraw->GetBuffer();
		pContext->VSSetConstantBuffers( CB_PerDraw::Slot, 1, &ppCBPerDraw );
		pContext->PSSetConstantBuffers( CB_PerDraw::Slot, 1, &ppCBPerDraw );

		GpuRTV* pHDRTarget = RenderResources::s_pTexHdrTarget->GetDeviceTexture()->LookupRTV(ResourceViewType::RenderTarget);
		pContext->OMSetRenderTargets(1, &pHDRTarget, nullptr);

		for (auto& pass : pTech->m_Passes)
		{
			Shader* pVS = pass.m_pVertexShader;
			Shader* pPS = pass.m_pPixelShader;

			IVertexLayout* pNewLayout = DeviceFactory::Get().GetOrCreateInputLayout(pVS->GetShaderBlob(), vtxFmt)->second;
			if (pNewLayout != pCurLayout)
			{
				pCurLayout = pNewLayout;
				pContext->IASetInputLayout(pCurLayout);
			}

			//pContext->VSSetShader(pVS., nullptr, 0);
			//pContext->PSSetShader(pPS, nullptr, 0);

			// Set samplers from pass
			// Set shader resources

			pContext->DrawIndexed(renderItem.pRenderMesh->GetIndicesCount(), 0, 0);
		}
	}

	gRenderer->PopProfileMarker("OPAQUE_PASS");
}

void RenderView::Execute_TranspPass()
{
	gRenderer->PushProfileMarker("TRANSPARENT_PASS");
	//for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::Transparent)])
	//{
		assert(false); // TODO: Not implemented
	//}
	gRenderer->PopProfileMarker("TRANSPARENT_PASS");
}

void RenderView::Execute_PostEffect()
{
	gRenderer->PushProfileMarker("POST_EFFECTS");

	for (auto& renderItem : m_renderLists[static_cast<int>(RenderListId::PostEffects)])
	{
		//gRenderer->PostEffectsMng()->Exec(Bloom);
		//gRenderer->PostEffectsMng()->Exec(ToneMapping);
		//gRenderer->PostEffectsMng()->Exec(AntiAliasing);
	}

	gRenderer->PopProfileMarker("POST_EFFECTS");
}
