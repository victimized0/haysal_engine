#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H
#pragma once

#include <platform.inl>
#include <WorldModule\IWorldEngine.h>

class WorldEngine;
extern WorldEngine* g_world;

class WorldEngine final : public IWorldEngine
{
public:
						WorldEngine();
	virtual				~WorldEngine();

	bool				Init() override;
	void				Release() final;
	void				ShutDown() final;
	void				Tick() override;
	void				Update() override;

	void				OnFrameStart() final;
	void				RenderWorld(const int renderFlags) override;
	//const Camera&		GetRenderingCamera() const override;

	void				RemoveAllStaticObjects() override;
	IRenderNode*		CreateRenderNode(RenderNodeType type) override;
	void				DeleteRenderNode(IRenderNode* pRenderNode) override;
	void				SelectEntity(IRenderNode* pEntity) override;
	void				RegisterEntity(IRenderNode* pEntity) override;
	void				UnregisterEntity(IRenderNode* pEntity) override;
	void				FreeRenderNodeState(IRenderNode* pEntity) override;
	uint32				GetObjectsByType(RenderNodeType objType, IRenderNode** pObjects = 0) override;

	ILightSourceNode*	CreateLightSource() override;
	void				DeleteLightSource(ILightSourceNode* pLightSource) override;
	float				GetLightAmountInRange(const Vec3& pPos, float fRange, bool bAccurate = 0) override;

	void				DeleteDecalsInRange(AABB* pAreaBox, IRenderNode* pEntity) override;
	void				DeleteEntityDecals(IRenderNode* pEntity) override;

	void				SetSkyColor(Vec3 vColor) override;
	void				SetSunColor(Vec3 vColor) override;
	Vec3				GetSkyColor() const override;
	Vec3				GetSunColor() const override;
	Vec3				GetSunDir() const override;
	Vec3				GetSunDirNormalized() const override;

	void				SetWind(const Vec3& vWind) override;
	Vec3				GetWind(const AABB& box, bool bIndoors) const override;
	Vec3				GetGlobalWind(bool bIndoors) const override;

	void				GetSkyLightParams(Vec3& sunDir, Vec3& sunIntensity, float& Km, float& Kr, float& g, Vec3& rgbWaveLengths) override;
	void				SetSkyLightParams(const Vec3& sunDir, const Vec3& sunIntensity, float Km, float Kr, float g, const Vec3& rgbWaveLengths, bool forceImmediateUpdate = false) override;
	
	void				SetRenderNodeMaterialAtPosition(RenderNodeType eNodeType, const Vec3& vPos, IMaterial* pMat) override;
	void				ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce) override;

private:
	std::vector<ILightSourceNode*>	m_staticLights;

};

#endif //WORLD_ENGINE_H
