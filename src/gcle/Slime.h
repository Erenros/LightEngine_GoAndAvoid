#pragma once

#include "Enemy.h"

class Slime : public Enemy
{
public:
    Slime();
    ~Slime() override;

    void OnInitialize() override;
    void OnCollisionEnter(Entity* other) override;

protected:
    void OnUpdate() override;

private:
    int m_StateFrames;
    float32 m_Speed;
};