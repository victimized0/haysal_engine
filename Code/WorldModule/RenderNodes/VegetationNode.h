#ifndef VEGETATION_RENDER_NODE_H
#define VEGETATION_RENDER_NODE_H
#pragma once

#include <WorldModule\IRenderNode.h>

class VegetationNode final : public IVegetationNode
{
public:
							VegetationNode();
	virtual					~VegetationNode();

	// Inherited via IVegetationNode
	virtual const char*		GetName() const final;
	virtual const char*		GetEntityClassName() const final;
	virtual Vec3			GetPos(bool bWorldOnly = true) const final;
	virtual const AABB		GetAABB() const final;
	virtual void			SetAABB(const AABB& aabb) final;
	virtual void			Translate(const Vec3& delta) final;
	virtual void			Render(const RenderParams& info, IRenderView* pRenderView) final;
	// ~Inherited via IVegetationNode

private:

};

#endif //VEGETATION_RENDER_NODE_H
