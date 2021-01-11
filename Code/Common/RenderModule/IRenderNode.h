#ifndef INTERFACE_RENDER_NODE_H
#define INTERFACE_RENDER_NODE_H
#pragma once

#include <RenderModule/IRenderer.h>

enum RenderNodeFlags : uint64
{
	eHidden = BIT64(0)
};

struct IRenderNode
{
	virtual				~IRenderNode() {}

	virtual const char* GetName() const = 0;
	virtual const char* GetEntityClassName() const = 0;

	virtual Vec3		GetPos(bool bWorldOnly = true) const = 0;
	virtual const AABB	GetAABB() const = 0;
	virtual void		FillAABB(AABB& aabb) const { aabb = GetAABB(); }
	virtual void		SetAABB(const AABB& aabb) = 0;

	virtual void		Translate(const Vec3& delta) = 0;
	virtual bool		IsVisible() const { return true; }

	virtual void		Render(const struct RenderInfo& info) = 0;
	virtual void        Release() { delete this; }

	virtual void		SetMatrix(const Matrix& mat) {}
	bool				IsHidden() const { return (GetFlags() & RenderNodeFlags::eHidden) ? true : false; }
	uint32				GetFlags() const { return m_flags; }

	//virtual void		SetOwnerEntity(IEntity* pEntity) { assert(!"Not supported by this object type"); }
	//virtual IEntity*	GetOwnerEntity() const { return nullptr; }

protected:
	virtual void		Hide(bool hide)
	{
		bool toggleHide = hide != IsHidden();
		if (toggleHide)
		{
			m_flags ^= RenderNodeFlags::eHidden;
			//if (m_pOcNode)
			//	m_pOcNode->ReorderObject(this, !hide);
		}
	}

	virtual void		Instance(bool instance) {};


private:
	//IOctreeNode*	m_pOcNode;
	IRenderNode*	m_pNext;
	IRenderNode*	m_pPrev;
	uint32			m_flags;
};

#endif //INTERFACE_RENDER_NODE_H
