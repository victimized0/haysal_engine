#ifndef INTERFACE_BASE_OBJECT_H
#define INTERFACE_BASE_OBJECT_H
#pragma once

struct IMaterial;
struct IRenderMesh;
struct IPhysEntity;
struct RenderParams;
struct RenderPassInfo;

struct IBaseObj
{
	virtual					~IBaseObj() {}

	virtual int				AddRef() = 0;
	virtual int				Release() = 0;
	virtual int				GetRefCount() const = 0;

	//virtual float			GetRadius() const { return GetAABB().GetRadius(); }
	virtual AABB			GetAABB() const = 0;
	virtual IMaterial*		GetMaterial() const = 0;
	virtual IRenderMesh*	GetRenderMesh() const = 0;
	virtual IPhysEntity*	GetPhysEntity() const = 0;

	virtual void			Render(const RenderParams& params, const RenderPassInfo& passInfo) = 0;
};

#endif //INTERFACE_BASE_OBJECT_H
