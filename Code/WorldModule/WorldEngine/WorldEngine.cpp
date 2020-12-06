#include "stdafx.h"
#include "WorldEngine.h"

#include <WorldModule\IWorldModule.h>

#include "RenderNodes\DecalRenderNode.h"
#include "RenderNodes\LightSourceNode.h"
#include "RenderNodes\MeshRenderNode.h"
#include "RenderNodes\VegetationNode.h"
#include "RenderNodes\CharacterRenderNode.h"

WorldEngine* g_world = nullptr;

WorldEngine::WorldEngine()
{
}

WorldEngine::~WorldEngine()
{

}

void WorldEngine::Release()
{
	if (this)
		delete this;
}

void WorldEngine::ShutDown()
{
}

void WorldEngine::OnFrameStart()
{
}

bool WorldEngine::Init()
{
	return true;
}

void WorldEngine::Update()
{
	UpdateCamera();
}

void WorldEngine::UpdateCamera()
{
	m_camera.CalculateMatrices();
}

void WorldEngine::RenderScene(const int renderFlags, const Camera& camera)
{
	IRenderer* pRnd = gEnv->pRenderer;
	if (pRnd == nullptr)
		return;

	std::unique_ptr<IRenderView> pRndView = std::move(pRnd->AllocateRenderView());
	pRnd->BeginFrame();

	BeginOcclusionCulling(pRndView.get());

	//if draw objects AABBs
	//	m_pObjectsTree->RenderDebug();

	m_objManager.RenderObjects(pRndView.get());

	// Notify game framework OnRenderScene();

	pRnd->RenderScene(pRndView.get());
	EndOcclusionCulling();
	pRnd->EndFrame();

	m_objManager.EndFrame();
}

void WorldEngine::BeginOcclusionCulling(IRenderView* pRenderView)
{
	m_objManager.BeginOcclusionCulling();
}

void WorldEngine::EndOcclusionCulling()
{
	m_objManager.FinishOcclusionCulling();
}

//const Camera& WorldEngine::GetRenderingCamera() const
//{
//
//}

IRenderNode* WorldEngine::CreateRenderNode(RenderNodeType type)
{
	switch (type)
	{
	case RenderNodeType::Decal:
		return new DecalRenderNode();
	case RenderNodeType::Light:
		return new LightSourceNode();
	case RenderNodeType::Mesh:
		return new MeshRenderNode();
	case RenderNodeType::Vegetation:
		return new VegetationNode();
	case RenderNodeType::Character:
		return new CharacterRenderNode();
	}
	return nullptr;
}

void WorldEngine::DeleteRenderNode(IRenderNode* pRenderNode)
{
}

void WorldEngine::SelectEntity(IRenderNode* pEntity)
{
}

void WorldEngine::RegisterEntity(IRenderNode* pEntity)
{
}

void WorldEngine::UnregisterEntity(IRenderNode* pEntity)
{
}

void WorldEngine::FreeRenderNodeState(IRenderNode* pEntity)
{
}

uint32 WorldEngine::GetObjectsByType(RenderNodeType objType, IRenderNode** pObjects)
{
	return 0;
}

ILightSourceNode* WorldEngine::CreateLightSource()
{
	return nullptr;
}

void WorldEngine::DeleteLightSource(ILightSourceNode* pLightSource)
{
}

float WorldEngine::GetLightAmountInRange(const Vec3& pPos, float fRange, bool bAccurate)
{
	return 0.0f;
}

void WorldEngine::DeleteDecalsInRange(AABB* pAreaBox, IRenderNode* pEntity)
{
}

void WorldEngine::DeleteEntityDecals(IRenderNode* pEntity)
{
}

void WorldEngine::SetSkyColor(Vec3 vColor)
{
}

void WorldEngine::SetSunColor(Vec3 vColor)
{
}

Vec3 WorldEngine::GetSkyColor() const
{
	return Vec3();
}

Vec3 WorldEngine::GetSunColor() const
{
	return Vec3();
}

Vec3 WorldEngine::GetSunDir() const
{
	return Vec3();
}

Vec3 WorldEngine::GetSunDirNormalized() const
{
	return Vec3();
}

void WorldEngine::SetWind(const Vec3& vWind)
{
}

Vec3 WorldEngine::GetWind(const AABB& box, bool bIndoors) const
{
	return Vec3();
}

Vec3 WorldEngine::GetGlobalWind(bool bIndoors) const
{
	return Vec3();
}

void WorldEngine::GetSkyLightParams(Vec3& sunDir, Vec3& sunIntensity, float& Km, float& Kr, float& g, Vec3& rgbWaveLengths)
{
}

void WorldEngine::SetSkyLightParams(const Vec3& sunDir, const Vec3& sunIntensity, float Km, float Kr, float g, const Vec3& rgbWaveLengths, bool forceImmediateUpdate)
{
}

void WorldEngine::SetRenderNodeMaterialAtPosition(RenderNodeType eNodeType, const Vec3& vPos, IMaterial* pMat)
{
}

void WorldEngine::ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce)
{
}

class WorldModule final : public IWorldModule
{
public:
	virtual ~WorldModule() { SAFE_RELEASE(gEnv->pWorld); }
	const char* GetName() const final { return "WorldModule"; }
	bool Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		ISystem* pSystem = env.pSystem;
		env.pWorld = g_world;
		return env.pWorld != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem* pSystem)
{
	g_world = new WorldEngine();
	auto pModule = new WorldModule();
	return pModule;
}
