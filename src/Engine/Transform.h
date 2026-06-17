#pragma once

#include <vector>

#include "MathGC.h"
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
    
<<<<<<< Updated upstream
    void SetDegAngle(Degrees angle);
    void SetRadAngle(Radians angle);
=======
    void SetDegAngle(gcle::Math::Degrees angle);
    void SetRadAngle(gcle::Math::Radians angle);
>>>>>>> Stashed changes

    void SetParent(Transform2D* pParent);
    
    void SetDirty();

public:
    Vector2f GetPosition();
    Vector2f GetDirection();

<<<<<<< Updated upstream
    Radians GetRadAngle();
    Degrees GetDegAngle();
=======
    gcle::Math::Radians GetRadAngle();
    gcle::Math::Degrees GetDegAngle();
>>>>>>> Stashed changes

    Transform2D* GetParent();
    Transform2D* GetChild(uint32 index);

    const Transform2D* GetParent() const;
    const Transform2D* GetChild(uint32 index) const;

    uint32 GetChildCount() const;

public:
    void AddChild(Transform2D* pChild);
    void RemoveParent();
    void RemoveChild(uint32 index);
<<<<<<< Updated upstream
    void Initialize(Vector2f m_Position, Degrees angle);
=======
    void Initialize(Vector2f m_Position, gcle::Math::Degrees angle);
>>>>>>> Stashed changes

private:
    Vector2f m_Position = {0,0};
    Vector2f m_Direction = { 0,0 };

<<<<<<< Updated upstream
    Degrees m_DegAngle = 0;
    Radians m_RadAngle = 0;
=======
    gcle::Math::Degrees m_DegAngle = 0;
    gcle::Math::Radians m_RadAngle = 0;
>>>>>>> Stashed changes

    Transform2D* mp_Parent = nullptr;
    std::vector<Transform2D*> mp_Childs;

    float m_DistanceFromParent = 0;
<<<<<<< Updated upstream
    Radians m_AngleDifferenceToParent = 0;
    Radians m_OffsetAngle = 0;

    Radians m_ParentAncientAngle = 0;
=======
    gcle::Math::Radians m_AngleDifferenceToParent = 0;
    gcle::Math::Radians m_OffsetAngle = 0;

    gcle::Math::Radians m_ParentAncientAngle = 0;
>>>>>>> Stashed changes
    
    bool m_IsDirty = false;

public:
    bool isSet = false;
};