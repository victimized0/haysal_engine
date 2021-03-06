#ifndef RENDER_VIEW_H
#define RENDER_VIEW_H
#pragma once

#include <RenderModule\IRenderView.h>

class RenderView : public IRenderView
{
public:
							RenderView();
	virtual					~RenderView();

	virtual void			SetFrameId(int frameId) override;
	virtual int				GetFrameId() const override;

	virtual void			SetViewport(const RenderViewport& viewport) override			{ m_viewport = viewport; }
	virtual const			RenderViewport& GetViewport() const override					{ return m_viewport;}

	virtual void			SetCameras(const Camera* pCameras, int camerasCount) override;
	virtual void			SetTargetClearColor(const Color& color) override;

	//virtual int				AddLight(const RenderLight& light) override;
	virtual int				GetLightsCount() const override;
	//virtual RenderLight&	GetLight(int lightId) override;

	virtual void			BeginFrame() override;

	virtual void			Submit(RenderItem item, RenderListId list) override;
	virtual void			ExecuteRenderPass(RenderListId listId) override;

	virtual bool			HasAddedItems(RenderListId listId) override;
	virtual size_t			NumAddedItems(RenderListId listId) override;

	virtual void			SetFrameData(void* pData, size_t size) final;

private:
	void					Execute_ShadowPass();
	void					Execute_EarlyZPass();
	void					Execute_LightsPass();
	void					Execute_OpaquePass();
	void					Execute_TranspPass();
	void					Execute_PostEffect();

private:
	std::array<RenderList, static_cast<size_t>(RenderListId::AACount)> m_renderLists;
	// TODO: Refactor to move code to PipelineStages array

	RenderViewport			m_viewport;

	ConstantBuffer*			m_pcbPerDraw;
	ConstantBuffer*			m_pcbPerMaterial;
	ConstantBuffer*			m_pcbPerPass;
	ConstantBuffer*			m_pcbPerShadowPass;
	ConstantBuffer*			m_pcbPerFrame;
};

#endif //RENDER_VIEW_H
