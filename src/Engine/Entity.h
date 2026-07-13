#pragma once

#include <unordered_map>
#include <unordered_set>

#include "GameObject.h"
#include "RigidBody.h"

class Collider;

class Entity : public GameObject
{
public:
    void Destroy();

    void SetTag(int32 tag);
    void SetRigidBody(bool isRigidBody);
    void SetStatic(bool isStatic);

    int64 GetId() const;
    int32* GetCollisionLayer();
    RigidBody2D& GetRigidBody();
    const RigidBody2D& GetRigidBody() const;

    bool IsStatic() const;
    bool IsColliding(Entity* other) const;
    bool IsTag(int32 tag) const;
    bool IsRigidBody() const;
    bool HasCollider() const;

    void AddCollider(Collider* collider);
    void RemoveCollider(Collider* collider);
    const std::unordered_set<Collider*>& GetColliders() const;

    Collider* CreateCollider(
        gcle::Shapes shape,
        bool isActive,
        Vector2f relativePosition,
        float32 relativeRotation,
        Vector2f relativeScale,
        bool isTrigger = false);

    bool IsWorldText() const;

    virtual void OnCollision(Entity* other) {}
    virtual void OnCollisionEnter(Entity* other) {}
    virtual void OnCollisionExit(Entity* other) {}

    virtual void OnTrigger(Entity* other) {}
    virtual void OnTriggerEnter(Entity* other) {}
    virtual void OnTriggerExit(Entity* other) {}

protected:
    Entity() = default;
    ~Entity() override;

    void OnUpdate() override {}
    void OnDestroy() override {}
    void OnInitialize() override {}

    bool m_IsWorldText = false;

private:
    void Initialize(gcle::Shapes shape) override;
    void Initialize();
    void Update(float32 dt) override;

private:
    int32 m_Tag = -1;
    bool m_IsStatic = false;
    bool m_IsHighlighted = false;

    RigidBody2D m_RigidBody;
    std::unordered_set<Collider*> mp_Colliders;
    std::unordered_map<int64, Entity*> m_CollidingEntity;
    std::unordered_map<int64, Entity*> m_TriggeringEntity;

    friend class Scene;
    friend class Camera;
    friend class GameManager;
    friend class PhysicsManager;
};
