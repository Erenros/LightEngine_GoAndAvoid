#pragma once
#include "Entity.h"

class Decor : public Entity
{
public:
    void SetWater(bool isWater, byte alpha = 140);
    bool IsWater() const { return m_IsWater; }

protected:
    void OnInitialize() override;

private:
    bool m_IsWater = false;
};