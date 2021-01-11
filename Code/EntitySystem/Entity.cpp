#include "stdafx.h"
#include "Entity.h"

#include <System\ISystem.h>
#include <WorldModule\IWorldEngine.h>
#include <WorldModule\IWorldObj.h>

Entity::Entity()
    : m_scale(0.0f)
    , m_rot(Quat::Identity)
    , m_pos(Vec3::Zero)
    , m_pAiAgent(nullptr)
{

}

Entity::~Entity()
{
    Release();
}

void Entity::Release()
{
    if (m_pAiAgent)
        gEnv->pAISystem->DestroyAgent(m_pAiAgent);

    if (m_pRenderNode)
        gEnv->pWorld->UnregisterEntity(m_pRenderNode);
}

Vec3 Entity::GetWorldPos()
{
    return GetLocalPos();
}

Quat Entity::GetWorldRot()
{
    return GetLocalRot();
}

float Entity::GetWorldScale()
{
    return GetLocalScale();
}

Matrix Entity::GetWorldMatrix()
{
    Matrix T = Matrix::CreateTranslation(GetWorldPos());
    Matrix R = Matrix::CreateFromQuaternion(GetWorldRot());
    Matrix S = Matrix::CreateScale(GetWorldScale());
    return T * R * S;
}

Matrix Entity::GetLocalMatrix()
{
    Matrix T = Matrix::CreateTranslation(m_pos);
    Matrix R = Matrix::CreateFromQuaternion(m_rot);
    Matrix S = Matrix::CreateScale(m_scale);
    return T * R * S;
}

void Entity::Translate(Vec3& delta)
{
    m_pos += delta;

    if (m_pRenderNode)
        m_pRenderNode->Translate(delta);
}

void Entity::Rotate(Quat& q)
{
    m_rot *= q;
}

void Entity::Parse(pugi::xml_node& entityNode)
{
    using namespace pugi;
    m_name = entityNode.attribute("name").as_string();

    // TODO: Parse physics proxy

    auto rpNode = entityNode.child("render_proxy");
    if (!rpNode.empty() && gEnv->pWorld)
    {
        auto objNode = rpNode.child("object");
        if (!objNode.empty())
        {
            // TODO: Parse render node type and pass it to CreateRenderNode() instead of hardcoding it
            m_pRenderNode = gEnv->pWorld->CreateRenderNode(RenderNodeType::Mesh);
            gEnv->pWorld->RegisterEntity(m_pRenderNode);

            const char* filename = objNode.attribute("src").as_string();
            IWorldObj* pObj = gEnv->pWorld->LoadWorldObj(filename);
            m_pRenderNode->SetWorldObj(pObj);
            // TODO: Parse material
        }
    }

    auto aiAgentNode = entityNode.child("ai_agent");
    if (!aiAgentNode.empty())
    {
        m_pAiAgent = gEnv->pAISystem->CreateAgent();
        m_pAiAgent->Parse(aiAgentNode);
        m_pAiAgent->SetOwner(this);
        m_pAiAgent->Init();
    }
}
