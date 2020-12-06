#ifndef MESH_RENDER_NODE_H
#define MESH_RENDER_NODE_H
#pragma once

#include <WorldModule\IRenderNode.h>

class MeshRenderNode final : public IMeshRenderNode
{
public:
							MeshRenderNode();
	virtual					~MeshRenderNode();

	// Inherited via IMeshRenderNode
	virtual const char*		GetName() const final;
	virtual const char*		GetEntityClassName() const final;
	virtual Vec3			GetPos(bool bWorldOnly = true) const final;
	virtual const AABB		GetAABB() const final;
	virtual void			SetAABB(const AABB& aabb) final;
	virtual void			Translate(const Vec3& delta) final;
	virtual void			Render(const RenderParams& info, IRenderView* pRenderView) final;
	// ~Inherited via IMeshRenderNode

private:

};

#endif //MESH_RENDER_NODE_H
