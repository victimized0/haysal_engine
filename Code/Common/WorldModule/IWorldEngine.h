#ifndef INTERFACE_WORLD_ENGINE_H
#define INTERFACE_WORLD_ENGINE_H
#pragma once

#include "IRenderNode.h"
#include "IMaterial.h"

struct IWorldEngine
{
	virtual							~IWorldEngine() {}

	virtual bool					Init() = 0;
	virtual void					Release() = 0;
	virtual void					ShutDown() = 0;

	virtual void					Update() = 0;
	virtual void					UpdateCamera() = 0;
	virtual void					OnFrameStart() = 0;
	virtual void					RenderScene(const int renderFlags, const Camera& camera) = 0;

	//virtual const					Camera& GetRenderingCamera() const = 0;

	//virtual int						SaveStatObj(IStatObj* pStatObj, TSerialize ser) = 0;
	//virtual IStatObj*				LoadStatObj(TSerialize ser) = 0;
	//virtual IStatObj*				CreateStatObj() = 0;
	//virtual IIndexedMesh*			CreateIndexedMesh() = 0;
	//virtual IStatObj*				LoadStatObj(const char* szFileName, const char* szGeomName = NULL, /*[Out]*/ IStatObj::SSubObject** ppSubObject = NULL) = 0;
	//virtual IStatObj*				FindStatObjectByFilename(const char* filename) = 0;
	virtual int						GetLoadedObjectCount() { return 0; }
	//virtual void					GetLoadedStatObjArray(IStatObj** pObjectsArray, int& nCount) = 0;

	//virtual void					AddObjToDebugDrawList(SObjectInfoToAddToDebugDrawList& objInfo) = 0;
	virtual IRenderNode* CreateRenderNode(RenderNodeType type) = 0;
	virtual void					DeleteRenderNode(IRenderNode* pRenderNode) = 0;
	virtual void					SelectEntity(IRenderNode* pEntity) = 0;
	virtual void					RegisterEntity(IRenderNode* pEntity) = 0;
	virtual void					UnregisterEntity(IRenderNode* pEntity) = 0;
	virtual void					FreeRenderNodeState(IRenderNode* pEntity) = 0;
	//virtual void					OnEntityDeleted(struct IEntity* pEntity) = 0;
	//virtual void					OnRenderMeshDeleted(IRenderMesh* pRenderMesh) = 0;
	virtual uint32					GetObjectsByType(RenderNodeType objType, IRenderNode** pObjects = 0) = 0;

	virtual ILightSourceNode* CreateLightSource() = 0;
	virtual void					DeleteLightSource(ILightSourceNode* pLightSource) = 0;
	//virtual void					GetLightVolumes(threadID nThreadID, SLightVolume*& pLightVols, uint32& nNumVols) = 0;
	virtual float					GetLightAmountInRange(const Vec3& pPos, float fRange, bool bAccurate = 0) = 0;

	//virtual const std::vector<ILightSource*>* GetLightEntities() = 0;
	//virtual std::vector<SRenderLight*>* GetDynamicLightSources() = 0;

	//virtual bool					IsUnderWater(const Vec3& vPos) const = 0;
	//virtual void					GetHDRSetupParams(Vec4 pParams[5]) const = 0;
	//virtual void					ResetParticlesAndDecals() = 0;
	//virtual void					CreateDecal(const CryEngineDecalInfo& Decal) = 0;
	virtual void					DeleteDecalsInRange(AABB* pAreaBox, IRenderNode* pEntity) = 0;
	virtual void					DeleteEntityDecals(IRenderNode* pEntity) = 0;

	//virtual void					GenerateFarTrees(const SRenderingPassInfo& passInfo) = 0;
	//virtual void					DrawFarTrees(const SRenderingPassInfo& passInfo) = 0;

	//virtual bool					IsSkyVisible() const = 0;
	//virtual eSkyType				GetSkyType() const = 0;
	//virtual IMaterial*				GetSkyMaterial() const = 0;
	//virtual void					SetSkyMaterial(IMaterial* pSkyMat, eSkyType type) = 0;

	virtual void					SetSkyColor(Vec3 vColor) = 0;
	virtual void					SetSunColor(Vec3 vColor) = 0;
	virtual Vec3					GetSkyColor() const = 0;
	virtual Vec3					GetSunColor() const = 0;
	virtual Vec3					GetSunDir()  const = 0;
	virtual Vec3					GetSunDirNormalized()  const = 0;

	virtual void					SetWind(const Vec3& vWind) = 0;
	virtual Vec3					GetWind(const AABB& box, bool bIndoors) const = 0;
	virtual Vec3					GetGlobalWind(bool bIndoors) const = 0;

	virtual void					GetSkyLightParams(Vec3& sunDir, Vec3& sunIntensity, float& Km, float& Kr, float& g, Vec3& rgbWaveLengths) = 0;
	virtual void					SetSkyLightParams(const Vec3& sunDir, const Vec3& sunIntensity, float Km, float Kr, float g, const Vec3& rgbWaveLengths, bool forceImmediateUpdate = false) = 0;

	//virtual void					SetMaxViewDistanceScale(float fScale) = 0;
	//virtual float					  GetMaxViewDistance(bool bScaled = true) = 0;

	virtual void					SetRenderNodeMaterialAtPosition(RenderNodeType eNodeType, const Vec3& vPos, struct IMaterial* pMat) = 0;

	//virtual ITerrain*				GetITerrain() = 0;
	//virtual ITerrain*				CreateTerrain(const STerrainInfo& TerrainInfo) = 0;
	//virtual void					DeleteTerrain() = 0;
	//virtual float					GetTerrainZ(float x, float y) = 0;
	//virtual float					GetTerrainElevation(float x, float y) = 0;
	//virtual Vec3					GetTerrainSurfaceNormal(Vec3 vPos) = 0;
	//virtual int						GetTerrainSize() = 0;
	//virtual void					SetTerrainSurfaceType(int x, int y, int nType) = 0;
	//virtual int						GetTerrainSectorSize() = 0;
	//virtual void					SetTerrainSectorTexture(const int nTexSectorX, const int nTexSectorY, unsigned int textureId) = 0;
	//virtual int						GetTerrainTextureNodeSizeMeters() = 0;

	virtual void					ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce) = 0;

};

#endif //INTERFACE_WORLD_ENGINE_H
