#pragma once

#include <vector>
#include <math.h>

#include "Math.hpp"
#include "include.h"

struct Transform2D
{
public: 
    Transform2D() = default;
    ~Transform2D();


    void RotateToDirection();
    void Forward();

    void UpdatePositionWithParentPosition();

public:
    void SetPosition(Vector2f);
    void SetDirection(Vector2f);
    
    void SetDegAngle(Math::Degrees angle);
    void SetRadAngle(Math::Radians angle);

    void SetParent(Transform2D* pParent);
    
    void SetDirty();

public:
    Vector2f GetPosition();
    Vector2f GetDirection();

    Math::Radians GetRadAngle();
    Math::Degrees GetDegAngle();

    Transform2D* GetParent();
    Transform2D* GetChild(uint32 index);

    const Transform2D* GetParent() const;
    const Transform2D* GetChild(uint32 index) const;

    uint32 GetChildCount() const;

public:
    void AddChild(Transform2D* pChild);
    void RemoveParent();
    void RemoveChild(uint32 index);
    void Initialize(Vector2f m_Position, Math::Degrees angle);

private:
    Vector2f m_Position = {0,0};
    Vector2f m_Direction = { 0,0 };

    Math::Degrees m_DegAngle = 0;
    Math::Radians m_RadAngle = 0;

    Transform2D* mp_Parent = nullptr;
    std::vector<Transform2D*> mp_Childs;

    float m_DistanceFromParent = 0;
    Math::Radians m_AngleDifferenceToParent = 0;
    Math::Radians m_OffsetAngle = 0;

    Math::Radians m_ParentAncientAngle = 0;
    
    bool m_IsDirty = false;

public:
    bool isSet = false;
};