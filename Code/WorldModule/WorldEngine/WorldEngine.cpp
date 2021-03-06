#include "stdafx.h"
#include "WorldEngine.h"

#include <WorldModule\IWorldModule.h>

#include "RenderNodes\DecalRenderNode.h"
#include "RenderNodes\LightSourceNode.h"
#include "RenderNodes\MeshRenderNode.h"
#include "RenderNodes\VegetationNode.h"
#include "RenderNodes\CharacterRenderNode.h"

#include "EntitySystem\IEntitySystem.h" // TODO: Delete this when normal level loading system is implemented!

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
	SAFE_DELETE(m_pRenderView);
}

void WorldEngine::OnFrameStart()
{
}

bool WorldEngine::Init()
{
	if (gEnv->pRenderer)
		m_pRenderView = gEnv->pRenderer->AllocateRenderView();

	LoadTestLevel();
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
	m_camera = camera;

	IRenderer* pRnd = gEnv->pRenderer;
	if (pRnd == nullptr)
		return;

	pRnd->BeginFrame();
	m_pRenderView->BeginFrame();

	BeginOcclusionCulling(m_pRenderView);

	//if draw objects AABBs
	//	m_pObjectsTree->RenderDebug();

	for (auto pNode: m_renderNodes)
	{
		MeshRenderNode* pMeshNode = reinterpret_cast<MeshRenderNode*>(pNode);
		m_objManager.RenderMeshNode(pMeshNode, m_pRenderView);
	}

	m_objManager.RenderObjects(m_pRenderView);

	// Notify game framework OnRenderScene();

	pRnd->RenderScene(m_pRenderView);
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
	UnregisterEntity(pRenderNode);
	SAFE_DELETE(pRenderNode);
}

void WorldEngine::SelectEntity(IRenderNode* pEntity)
{
}

void WorldEngine::RegisterEntity(IRenderNode* pEntity)
{
	m_renderNodes.push_back(pEntity);
}

void WorldEngine::UnregisterEntity(IRenderNode* pEntity)
{
	auto it = std::find(m_renderNodes.begin(), m_renderNodes.end(), pEntity);
	if (it != m_renderNodes.end())
		m_renderNodes.erase(it);
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

IIndexedMesh* WorldEngine::CreateIndexedMesh()
{
	return new IndexedMesh();
}

IWorldObj* WorldEngine::CreateWorldObj()
{
	WorldObject* pWorldObj = new WorldObject();
	pWorldObj->CreateIndexedMesh();
	return pWorldObj;
}

IWorldObj* WorldEngine::LoadWorldObj(const char* filename, const char* geomName)
{
	if (filename == nullptr)
		return nullptr;
	if (filename[0] == 0)
		return nullptr;

	return m_objManager.LoadObject(filename);
}

IWorldObj* WorldEngine::FindWorldObjByName(const char* filename)
{
	if (filename == nullptr)
		return nullptr;
	if (filename[0] == 0)
		return nullptr;

	return m_objManager.FindObject(filename);
}

int WorldEngine::GetLoadedObjectCount()
{
	return m_objManager.GetLoadedCount();
}

void WorldEngine::LoadTestLevel()
{
	// TODO: Delete this method, it's used only for test purposes.
	using namespace pugi;

	//gEnv->pEntitySystem->Load();

	xml_document doc = gEnv->pSystem->LoadXmlFromFile("Data/Levels/Test/test.xml");

	auto firstObject = doc.root().find_node([&](xml_node& node) { return strcmp(node.name(), "object") == 0; });
	for (xml_node& object = firstObject; object; object = object.next_sibling())
	{
		const char* fileName = object.attribute("filename").as_string();
		m_objManager.LoadObject(fileName);
	}

	auto firstEntity = doc.root().find_node([&](xml_node& node) { return strcmp(node.name(), "entity") == 0; });
	for (xml_node& entity = firstEntity; entity; entity = entity.next_sibling())
	{
		const char* name = entity.attribute("name").as_string();
		gEnv->pEntitySystem->LoadEntity(name);
	}
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
