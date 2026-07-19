#pragma once
#include "Entity.h"

class Enemy : public Entity
{
protected:
    void OnInitialize() override;
    void OnCollisionEnter(Entity* other) override;
};