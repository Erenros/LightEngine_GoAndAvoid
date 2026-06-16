#pragma once

#include <vector>
#include <math.h>

#include "Math.hpp"
#include "include.h"
#include "Entity.h"

struct Transform2D
{
private: 
    Vector2f m_Position;
    Vector2f m_Direction;
    Math::Degrees m_DegAngle;
    Math::Radians m_RadAngle;

    bool m_IsDirty;
        
    float m_DistanceFromParent;
    Math::Radians m_AngleDifferenceToParent;
    Math::Radians m_OffsetAngle;

    Math::Radians m_ParentAncientAngle;

    Transform2D* mp_Parent;
    std::vector<Transform2D*> mp_Childs;

public: 

    void UpdateAngleWithDirection();
    void UpdateDirectionWithAngle();

    void SetParent(Transform2D* pParent);
    void AddChild(Transform2D* pChild);

    Transform2D* GetParent();
    Transform2D* GetChild(uint32 index);

    const Transform2D* GetParent() const;
    const Transform2D* GetChild(uint32 index) const;

    void RemoveParent();
    void RemoveChild(uint32 index);

    uint32 GetChildCount() const;

    void SetDirty();

    void UpdatePositionWithParentPosition();

    void SetPosition(Vector2f);
    Vector2f GetPosition();

    void SetDirection(Vector2f);
    Vector2f GetDirection();

    void SetDegAngle(Math::Degrees angle);
    Math::Degrees GetDegAngle();

    void SetRadAngle(Math::Radians angle);
    Math::Radians GetRadAngle();
};