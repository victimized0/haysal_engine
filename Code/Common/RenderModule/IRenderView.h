#ifndef INTERFACE_RENDER_VIEW_H
#define INTERFACE_RENDER_VIEW_H
#pragma once

struct RenderViewport;
struct RenderItem;
class Camera;

typedef std::vector<RenderItem> RenderList;

enum class RenderListId : uint8
{
	Invalid		= 0,
	ShadowGen	,
	ZPrePass	,
	LightPass	,
	Opaque		,
	Transparent	,
	PostEffects ,

	Count
};

struct IRenderView
{
	virtual							~IRenderView() {}

	virtual void					SetFrameId(int frameId) = 0;
	virtual int						GetFrameId() const = 0;
	virtual void					SetDeviceContext(GpuContext* pContext) = 0;

	virtual void					SetViewport(const RenderViewport& viewport) = 0;
	virtual const RenderViewport&	GetViewport() const = 0;
	virtual void					SetCameras(const Camera* pCameras, int camerasCount) = 0;
	virtual void					SetTargetClearColor(const Color& color) = 0;

	//virtual int						AddLight(const RenderLight& light) = 0;
	virtual int						GetLightsCount() const = 0;
	//virtual RenderLight&			GetLight(int lightId) = 0;

	virtual void					Submit(RenderItem item, RenderListId listId) = 0;
	virtual void					ExecuteRenderPass(RenderListId listId) = 0;

	virtual bool					HasAddedItems(RenderListId listId) = 0;
	virtual size_t					NumAddedItems(RenderListId listId) = 0;
};

#endif //INTERFACE_RENDER_VIEW_H
