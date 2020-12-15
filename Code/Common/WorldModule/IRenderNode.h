#ifndef INTERFACE_RENDER_NODE_H
#define INTERFACE_RENDER_NODE_H
#pragma once

#include <RenderModule\IRenderer.h>

enum class RenderNodeType
{
	NonRenderable,
	Vegetation,
	Light,
	Mesh,
	Decal,
	Character,

	Count
};

enum RenderNodeFlags : uint64
{
	Hidden = BIT64(0)
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

	virtual void		Render(const RenderParams& info, struct IRenderView* pRenderView) = 0;
	virtual void        Release() { delete this; }

	virtual void		SetMatrix(const Matrix& mat) {}
	bool				IsHidden() const { return (GetFlags() & RenderNodeFlags::Hidden) ? true : false; }
	uint32				GetFlags() const { return m_flags; }

	//virtual void		SetOwnerEntity(IEntity* pEntity) { assert(!"Not supported by this object type"); }
	//virtual IEntity*	GetOwnerEntity() const { return nullptr; }

protected:
	virtual void		Hide(bool hide)
	{
		if (hide)
			m_flags |= RenderNodeFlags::Hidden;
		else
			m_flags &= ~RenderNodeFlags::Hidden;
	}

	virtual void		Instance(bool instance) {};


private:
	//IOctreeNode*	m_pOcNode;
	IRenderNode*	m_pNext;
	IRenderNode*	m_pPrev;
	uint32			m_flags;
};

struct IVegetationNode : public IRenderNode
{
	virtual ~IVegetationNode() {}
};

struct ILightSourceNode : public IRenderNode
{
	virtual ~ILightSourceNode() {}
};

struct IDecalRenderNode : public IRenderNode
{
	virtual ~IDecalRenderNode() {}
};

struct IMeshRenderNode : public IRenderNode
{
	virtual ~IMeshRenderNode() {}
};

struct ICharacterRenderNode : public IRenderNode
{
	virtual ~ICharacterRenderNode() {}
};

#endif //INTERFACE_RENDER_NODE_H
