#ifndef DECAL_RENDER_NODE_H
#define DECAL_RENDER_NODE_H
#pragma once

#include <WorldModule\IRenderNode.h>
struct IRenderView;

class DecalRenderNode final : public IDecalRenderNode
{
public:
							DecalRenderNode();
	virtual					~DecalRenderNode();

	// Inherited via IDecalRenderNode
	virtual const char*		GetName() const final;
	virtual const char*		GetEntityClassName() const final;
	virtual Vec3			GetPos(bool bWorldOnly = true) const final;
	virtual const AABB		GetAABB() const final;
	virtual void			SetAABB(const AABB& aabb) final;
	virtual void			Translate(const Vec3& delta) final;
	virtual void			Render(const RenderParams& info, IRenderView* pRenderView) final;
	virtual void			SetMaterial(IMaterial* pMaterial) final;
	// ~Inherited via IDecalRenderNode

private:

};

#endif //DECAL_RENDER_NODE_H
