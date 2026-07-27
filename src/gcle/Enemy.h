#pragma once
#include "KillableEntity.h"

class Enemy : public KillableEntity
{
protected:
    void OnCollisionEnter(Entity* other) override;
    void OnDeath() override;

private:
    int32 m_scoreReward = 100;
};