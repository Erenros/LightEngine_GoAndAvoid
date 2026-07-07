#include "Entity.h"
#include "Core/InputManager.h"
#include "RessourceManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Collider.h"

static int64 sId = 0;

Entity::~Entity() {
	if (mp_RenderShape != nullptr) {
		delete mp_RenderShape;
		mp_RenderShape = nullptr;
	}

	for (auto& collider : mp_Colliders)
	{
		delete collider;
	}
	mp_Colliders.clear();

}

void Entity::Initialize(gcle::Shapes shape)
{
	GameObject::Initialize(shape);

	m_Tag = -1;

	m_RigidBody.Initialize(&m_Transform);
	m_RigidBody.SetActive(true);

	OnInitialize();

	m_Id = sId++;

	m_Mask = 1;
}

void Entity::Initialize()
{
	m_Tag = -1;

	m_RigidBody.Initialize(&m_Transform);
	m_RigidBody.SetActive(true);

	OnInitialize();

	m_Id = sId++;

	m_Mask = 1;

}

void Entity::Update(float32 dt)
{
	if (m_RigidBody.IsActive())
		m_RigidBody.Update(dt);

	GameObject::Update(dt);

	OnUpdate();
}

void Entity::AddCollider(Collider* pCollider)
{
	if (pCollider == nullptr)
		return;

	pCollider->SetOwner(this);
	mp_Colliders.insert(pCollider);
}

void Entity::RemoveCollider(Collider* pCollider)
{
	if (pCollider == nullptr)
		return;

	mp_Colliders.erase(pCollider);
}

Collider* Entity::CreateCollider(gcle::Shapes shape, bool isActive, Vector2f relativePosition, float32 relativeRotation, Vector2f relativeScale)
{
	Collider* collider = GCLE_NEW Collider();

	gcle::Shape* colliderShape = GetBaseShape(shape);

	Vector2f entityScale = m_Transform.GetScale();
	colliderShape->SetScale({ relativeScale.x * entityScale.x, relativeScale.y * entityScale.y });

	collider->Initialize(colliderShape, m_Transform.GetPosition() + relativePosition, m_Transform.GetDegAngle() + relativeRotation, this);

	AddCollider(collider);
	collider->SetActive(isActive);

	return collider;
}

void Entity::Destroy()
{
	if (this != nullptr)
	{
		m_ToDestroy = true;
		PhysicsManager::GetInstance().RemoveEntity(this);
		OnDestroy();
	}
}

void Entity::SetRigidBody(bool isRigidBody)
{
	m_RigidBody.SetActive(isRigidBody);

	if (isRigidBody)
	{
		PhysicsManager::GetInstance().AddEntity(this);
		m_isHighlighted = true;
	}

	if (!isRigidBody)
	{
		PhysicsManager::GetInstance().RemoveEntity(this);
		m_isHighlighted = false;
	}
}

void Entity::SetStatic(bool isStatic)
{
	m_isStatic = isStatic;
}

bool Entity::HasCollider()
{
	std::cout << mp_Colliders.size() << std::endl;
	return mp_Colliders.size() > 0;
}

bool Entity::IsStatic() const
{
	return !m_isStatic;
}

bool Entity::IsColliding(Entity* other)
{
	for (Collider* pCollider : mp_Colliders)
	{
		if (!pCollider->IsActive())
			continue;

		for (Collider* pOtherCollider : other->mp_Colliders)
		{
			if (!pOtherCollider->IsActive())
				continue;

			if (PhysicsManager::GetInstance().IsColliding(pCollider, pOtherCollider))
				return true;
		}
	}

	return false;
}

