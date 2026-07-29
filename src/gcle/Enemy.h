#pragma once
#include "KillableEntity.h"
#include <functional>

class Enemy : public KillableEntity
{
public:
    void SetOnDestroyedCallback(std::function<void()> callback) { m_onDestroyedCallback = std::move(callback); }

protected:
    void OnUpdate() override final;
    virtual void OnAliveUpdate() {}

    void OnCollisionEnter(Entity* other) override;
    void OnDeath() override;
    void OnDestroy() override;

private:
    void StartDeathSequence();
    void UpdateDeathAnimation(float32 dt);
    bool IsOffScreen();

private:
    int32 m_scoreReward = 100;

    bool m_isDying = false;
    float32 m_deathElapsedTime = 0.0f;
    float32 m_deathFallSpeed = 0.0f;

    std::function<void()> m_onDestroyedCallback;
};