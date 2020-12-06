#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H
#pragma once

#include <platform.inl>
#include <WorldModule\IWorldEngine.h>
#include "Object\ObjectManager.h"

class WorldEngine;
class RenderView;
extern WorldEngine* g_world;

class WorldEngine final : public IWorldEngine
{
public:
									WorldEngine();
	virtual							~WorldEngine();

	virtual bool					Init() override;
	virtual void					Release() final;
	virtual void					ShutDown() final;
	virtual void					Update() override;
	virtual void					UpdateCamera() override;

	virtual void					OnFrameStart() final;
	virtual void					RenderScene(const int renderFlags, const Camera& camera) override;
	virtual //const Camera&			GetRenderingCamera() const override;

	virtual void					BeginOcclusionCulling(IRenderView* pRenderView);
	virtual void					EndOcclusionCulling();

	virtual IRenderNode*			CreateRenderNode(RenderNodeType type) override;
	virtual void					DeleteRenderNode(IRenderNode* pRenderNode) override;
	virtual void					SelectEntity(IRenderNode* pEntity) override;
	virtual void					RegisterEntity(IRenderNode* pEntity) override;
	virtual void					UnregisterEntity(IRenderNode* pEntity) override;
	virtual void					FreeRenderNodeState(IRenderNode* pEntity) override;
	virtual uint32					GetObjectsByType(RenderNodeType objType, IRenderNode** pObjects = 0) override;

	virtual ILightSourceNode*		CreateLightSource() override;
	virtual void					DeleteLightSource(ILightSourceNode* pLightSource) override;
	virtual float					GetLightAmountInRange(const Vec3& pPos, float fRange, bool bAccurate = 0) override;

	virtual void					DeleteDecalsInRange(AABB* pAreaBox, IRenderNode* pEntity) override;
	virtual void					DeleteEntityDecals(IRenderNode* pEntity) override;

	virtual void					SetSkyColor(Vec3 vColor) override;
	virtual void					SetSunColor(Vec3 vColor) override;
	virtual Vec3					GetSkyColor() const override;
	virtual Vec3					GetSunColor() const override;
	virtual Vec3					GetSunDir() const override;
	virtual Vec3					GetSunDirNormalized() const override;

	virtual void					SetWind(const Vec3& vWind) override;
	virtual Vec3					GetWind(const AABB& box, bool bIndoors) const override;
	virtual Vec3					GetGlobalWind(bool bIndoors) const override;

	virtual void					GetSkyLightParams(Vec3& sunDir, Vec3& sunIntensity, float& Km, float& Kr, float& g, Vec3& rgbWaveLengths) override;
	virtual void					SetSkyLightParams(const Vec3& sunDir, const Vec3& sunIntensity, float Km, float Kr, float g, const Vec3& rgbWaveLengths, bool forceImmediateUpdate = false) override;

	virtual void					SetRenderNodeMaterialAtPosition(RenderNodeType eNodeType, const Vec3& vPos, IMaterial* pMat) override;
	virtual void					ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce) override;

private:
	Camera							m_camera;
	std::vector<ILightSourceNode*>	m_staticLights;
	ObjectManager					m_objManager;
	//class LightEntity*			m_pSun;
	//class OctreeNode*				m_pObjectsTree = nullptr;

};

#endif //WORLD_ENGINE_H
