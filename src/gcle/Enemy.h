#pragma once
#include "KillableEntity.h"

class Enemy : public KillableEntity
{
protected:
    void OnCollisionEnter(Entity* other) override;
};