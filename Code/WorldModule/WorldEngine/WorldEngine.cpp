#include "stdafx.h"
#include "WorldEngine.h"

#include <WorldModule\IWorldModule.h>

WorldEngine* g_world = nullptr;

WorldEngine::WorldEngine()
{
}

WorldEngine::~WorldEngine()
{
}

void WorldEngine::Release()
{
}

void WorldEngine::ShutDown()
{
}

void WorldEngine::OnFrameStart()
{
}

bool WorldEngine::Init()
{
	return false;
}

void WorldEngine::Update()
{
}

void WorldEngine::Tick()
{
}

void WorldEngine::RenderWorld(const int renderFlags)
{
}

const Camera& WorldEngine::GetRenderingCamera() const
{
	// TODO: insert return statement here
}

void WorldEngine::RemoveAllStaticObjects()
{
}

IRenderNode* WorldEngine::CreateRenderNode(RenderNodeType type)
{
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

uint32 WorldEngine::GetObjectsByType(RenderNodeType objType, IRenderNode** pObjects = 0)
{
	return uint32();
}

ILightSource* WorldEngine::CreateLightSource()
{
	return nullptr;
}

void WorldEngine::DeleteLightSource(ILightSource* pLightSource)
{
}

float WorldEngine::GetLightAmountInRange(const Vec3& pPos, float fRange, bool bAccurate = 0)
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

void WorldEngine::SetSkyLightParams(const Vec3& sunDir, const Vec3& sunIntensity, float Km, float Kr, float g, const Vec3& rgbWaveLengths, bool forceImmediateUpdate = false)
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
