#ifndef DECAL_RENDER_NODE_H
#define DECAL_RENDER_NODE_H
#pragma once

#include <WorldModule\IRenderNode.h>

class DecalRenderNode : public IDecalRenderNode
{
public:
					DecalRenderNode();
	virtual			~DecalRenderNode();

	// Inherited via IDecalRenderNode
	const char*		GetName() const override;
	const char*		GetEntityClassName() const override;
	Vec3			GetPos(bool bWorldOnly = true) const override;
	const AABB		GetAABB() const override;
	void			SetAABB(const AABB& aabb) override;
	void			Translate(const Vec3& delta) override;
	void			Render(const RenderInfo& info) override;

private:

};

#endif //DECAL_RENDER_NODE_H
