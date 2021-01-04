#ifndef MESH_RENDER_NODE_H
#define MESH_RENDER_NODE_H
#pragma once

#include <EntitySystem\IEntity.h>
#include <WorldModule\IRenderNode.h>

class MeshRenderNode final : public IMeshRenderNode
{
public:
							MeshRenderNode();
	virtual					~MeshRenderNode();

	// Inherited via IMeshRenderNode
	virtual const char*		GetName() const final;
	virtual const char*		GetEntityClassName() const final { return "MeshRenderNode"; }

	virtual Vec3			GetPos(bool bWorldOnly = true) const final;

	virtual const AABB		GetAABB() const final			{ return m_bbox; }
	virtual void			SetAABB(const AABB& aabb) final { m_bbox = aabb; }

	void					SetWorldObj(IWorldObj* pObj);
	virtual void			SetMaterial(IMaterial* pMaterial) final;
	virtual void			SetMatrix(const Matrix& mat) final;
	virtual const Matrix&	GetMatrix() const final			{ return m_matrix; }

	virtual void			Translate(const Vec3& delta) final;
	virtual void			Render(const RenderParams& info, IRenderView* pRenderView) final;

	virtual void			SetOwnerEntity(IEntity* pEntity) final	{ m_pOwner = pEntity; }
	virtual IEntity*		GetOwnerEntity() const final			{ return m_pOwner; }
	// ~Inherited via IMeshRenderNode

private:
	void					CalculateBBox();

private:
	IEntity*				m_pOwner;
	WorldObject*			m_pWorldObj;
	Matrix					m_matrix;
	IMaterial*				m_pMaterial;
	AABB					m_bbox;

};

#endif //MESH_RENDER_NODE_H
