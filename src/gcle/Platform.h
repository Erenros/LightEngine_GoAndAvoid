#pragma once
#include "Entity.h"
#include <string>

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
    PlatformType GetType() const { return m_type; }
    bool IsSpikes() const { return m_type == PlatformType::Spikes; }

    void SetStatic(bool isStatic);
    void BuildColliders(const std::string& slopeType, float engineScale);

protected:
    void OnInitialize() override;

private:
    PlatformType m_type = PlatformType::Grass;
};