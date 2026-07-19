#pragma once
#include "Entity.h"

enum class PlatformType
{
    Grass,
    Dirt,
    Wood,
    Stone,
    Spikes
};

class Platform : public Entity
{
public:
    void SetType(PlatformType type);

protected:
    void OnInitialize() override;

private:
    PlatformType m_type = PlatformType::Grass;
};