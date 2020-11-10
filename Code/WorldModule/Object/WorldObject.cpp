#include "stdafx.h"
#include "WorldObject.h"

#include "IndexedMesh.h"
#include "ObjLoader\ObjLoader.h"

WorldObject::WorldObject()
    : m_pParent(nullptr)
    , m_pMaterial(nullptr)
    , m_pRenderMesh(nullptr)
    , m_isSubObj(false)
    , m_canUnload(false)
    , m_loadedTrisCount(0)
    , m_loadedVertsCount(0)
    , m_flags(0)
    , m_refCount(0)
{
}

WorldObject::~WorldObject()
{
}

void WorldObject::Init()
{
}

void WorldObject::ShutDown()
{
}

int WorldObject::AddRef()
{
    return 0;
}

int WorldObject::Release()
{
    return 0;
}

IIndexedMesh* WorldObject::GetIndexedMesh(bool createfNone)
{
    return nullptr;
}

IIndexedMesh* WorldObject::CreateIndexedMesh()
{
    return nullptr;
}

void WorldObject::SetRenderMesh(std::unique_ptr<IRenderMesh>&& pRenderMesh)
{
    if (m_pRenderMesh == pRenderMesh)
        return;

    m_pRenderMesh = std::move(pRenderMesh);

    if (m_pRenderMesh)
    {
        m_loadedTrisCount = m_pRenderMesh->GetIndicesCount() / 3;
        m_loadedVertsCount = m_pRenderMesh->GetVerticesCount();
    }
}

bool WorldObject::Load(const char* filepath)
{
    if (m_isSubObj)
        return true;

    bool result = false;

    std::string ext = Path::GetExtension(filepath);
    if (ext == "obj")
    {
        result = ObjLoader::Load(filepath, m_pIndexedMesh.get());
    }
    else
    {
        assert(false);
        // Not implemented
    }

    return result;
}

void WorldObject::Refresh(uint32 flags)
{
}

void WorldObject::Invalidate(bool bPhysics)
{
}

void WorldObject::Render(const RenderParams& params, const RenderPassInfo& passInfo)
{
    RenderItem rendItem;
    rendItem.pRenderMesh = m_pRenderMesh.get();

    if (m_flags & ObjectFlags::CastShadows)
        passInfo.Passes[PassId::Shadow].Submit(rendItem);
    if (!(m_flags & ObjectFlags::NoDepth))
        passInfo.Passes[PassId::Depth].Submit(rendItem);
    if (m_flags & ObjectFlags::Glow)
        passInfo.Passes[PassId::Light].Submit(rendItem);
    if (!(m_flags & ObjectFlags::Unlit))
        passInfo.Passes[PassId::Color].Submit(rendItem);
}

IWorldObj::SubObj* WorldObject::FindSubObject(const char* nodeName)
{
    return nullptr;
}

IWorldObj::SubObj& WorldObject::AddSubObject(IWorldObj* pObj)
{
    // TODO: insert return statement here
}

bool WorldObject::RemoveSubObject(int index)
{
    return false;
}

void WorldObject::SetSubObjectsCount(int count)
{
}

IWorldObj::SubObj* WorldObject::GetSubObject(int index)
{
    if (index >= 0 && index < static_cast<int>(m_subObjects.size()))
        return &m_subObjects[index];
    else
        return nullptr;
}

IWorldObj* WorldObject::UpdateVertices(std::vector<Vec3>& pVertices, std::vector<Vec3>&& pNormals, int iVtx0, int nVtx)
{
    return nullptr;
}

IWorldObj* WorldObject::SkinVertices(std::vector<Vec3>& pSkelVtx, const Matrix& mtxSkelToMesh)
{
    return nullptr;
}

void WorldObject::DebugDraw()
{
}

void WorldObject::GetStatistics(Statistics& stats)
{
}
