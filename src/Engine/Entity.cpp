#include "Entity.h"

#include "Collider.h"
#include "PhysicsManager.h"

namespace
{
    int64 s_NextEntityId = 0;
}

Entity::~Entity()
{
    PhysicsManager::GetInstance().RemoveEntity(this);

    for (Collider* collider : mp_Colliders)
        delete collider;

    mp_Colliders.clear();
}

void Entity::Initialize(gcle::Shapes shape)
{
    GameObject::Initialize(shape);
    Initialize();
}

void Entity::Initialize()
{
    m_Tag = -1;
    m_ToDestroy = false;
    m_Mask = 1;
    m_Id = s_NextEntityId++;

    m_RigidBody.Initialize(&m_Transform);
    m_RigidBody.SetActive(true);

    OnInitialize();
}

void Entity::Update(float32 dt)
{
    if (m_RigidBody.IsActive())
        m_RigidBody.Update(dt);

    GameObject::Update(dt);
    OnUpdate();
}

void Entity::Destroy()
{
    if (m_ToDestroy)
        return;

    m_ToDestroy = true;
    PhysicsManager::GetInstance().RemoveEntity(this);
    OnDestroy();
}

void Entity::SetTag(int32 tag)
{
    m_Tag = tag;
}

void Entity::SetRigidBody(bool isRigidBody)
{
    m_RigidBody.SetActive(isRigidBody);
    m_IsHighlighted = isRigidBody;

    if (isRigidBody)
        PhysicsManager::GetInstance().AddEntity(this);
    else
        PhysicsManager::GetInstance().RemoveEntity(this);
}

void Entity::SetStatic(bool isStatic)
{
    m_IsStatic = isStatic;
}

int64 Entity::GetId() const
{
    return m_Id;
}

int32* Entity::GetCollisionLayer()
{
    return &m_Mask;
}

RigidBody2D& Entity::GetRigidBody()
{
    return m_RigidBody;
}

const RigidBody2D& Entity::GetRigidBody() const
{
    return m_RigidBody;
}

bool Entity::IsStatic() const
{
    return m_IsStatic;
}

bool Entity::IsColliding(Entity* other) const
{
    if (other == nullptr)
        return false;

    for (Collider* collider : mp_Colliders)
    {
        if (collider == nullptr || !collider->IsActive())
            continue;

        for (Collider* otherCollider : other->mp_Colliders)
        {
            if (otherCollider == nullptr || !otherCollider->IsActive())
                continue;

            if (PhysicsManager::GetInstance().IsColliding(collider, otherCollider))
                return true;
        }
    }

    return false;
}

bool Entity::IsTag(int32 tag) const
{
    return m_Tag == tag;
}

bool Entity::IsRigidBody() const
{
    return m_RigidBody.IsActive();
}

bool Entity::HasCollider() const
{
    return !mp_Colliders.empty();
}

void Entity::AddCollider(Collider* collider)
{
    if (collider == nullptr)
        return;

    const bool hadNoCollider = mp_Colliders.empty();

    collider->SetOwner(this);
    mp_Colliders.insert(collider);

    if (hadNoCollider)
        PhysicsManager::GetInstance().AddEntity(this);
}

void Entity::RemoveCollider(Collider* collider)
{
    if (collider == nullptr)
        return;

    mp_Colliders.erase(collider);

    if (mp_Colliders.empty() && !m_RigidBody.IsActive())
        PhysicsManager::GetInstance().RemoveEntity(this);
}

const std::unordered_set<Collider*>& Entity::GetColliders() const
{
    return mp_Colliders;
}

Collider* Entity::CreateCollider(
    gcle::Shapes shape,
    bool isActive,
    Vector2f relativePosition,
    float32 relativeRotation,
    Vector2f relativeScale,
    bool isTrigger)
{
    gcle::Shape* colliderShape = GetBaseShape(shape);
    if (colliderShape == nullptr)
        return nullptr;

    const Vector2f entityScale = m_Transform.GetScale();
    colliderShape->SetScale({
        relativeScale.x * entityScale.x,
        relativeScale.y * entityScale.y
        });

    Collider* collider = GCLE_NEW Collider();
    collider->SetTrigger(isTrigger);
    collider->Initialize(
        colliderShape,
        m_Transform.GetPosition() + relativePosition,
        m_Transform.GetDegAngle() + relativeRotation,
        this);

    AddCollider(collider);
    collider->SetActive(isActive);

    return collider;
}

bool Entity::IsWorldText() const
{
    return m_IsWorldText;
}
