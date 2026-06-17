#include "Transform.h"

Transform2D::~Transform2D() {

    if (mp_Parent != nullptr) {
        for (auto it = mp_Parent->mp_Childs.begin(); it != mp_Parent->mp_Childs.end(); ++it) {
            if (*it == this) {
                mp_Parent->mp_Childs.erase(it);
                break;
            }
        }
    }

    for (auto pChild : mp_Childs) {
        pChild->SetParent(nullptr);
    }
}

<<<<<<< Updated upstream
void Transform2D::Initialize(Vector2f position, Degrees angle)
{
    m_Position = position;
    m_DegAngle = angle;
    m_RadAngle = MathGC::DegToRad(m_DegAngle);
=======
void Transform2D::Initialize(Vector2f position, gcle::Math::Degrees angle)
{
    m_Position = position;
    m_DegAngle = angle;
    m_RadAngle = gcle::Math::DegToRad(m_DegAngle);
>>>>>>> Stashed changes

    isSet = true;
}

#pragma region Gets

Transform2D* Transform2D::GetParent() {
    return mp_Parent;
}

Transform2D* Transform2D::GetChild(uint32 index) {
    return mp_Childs[index];
}

const Transform2D* Transform2D::GetParent() const {
    return mp_Parent;
}

const Transform2D* Transform2D::GetChild(uint32 index) const {
    return mp_Childs[index];
}

uint32 Transform2D::GetChildCount() const {
    return mp_Childs.size();
}

Vector2f Transform2D::GetPosition()
{
    return m_Position;
}

Vector2f Transform2D::GetDirection()
{
    return m_Direction;
}

<<<<<<< Updated upstream
Degrees Transform2D::GetDegAngle()
=======
gcle::Math::Degrees Transform2D::GetDegAngle()
>>>>>>> Stashed changes
{
    return m_DegAngle;
}

<<<<<<< Updated upstream
Radians Transform2D::GetRadAngle()
=======
gcle::Math::Radians Transform2D::GetRadAngle()
>>>>>>> Stashed changes
{
    return m_RadAngle;
}
#pragma endregion

#pragma region Sets

void Transform2D::SetParent(Transform2D* pParent) {
    if (pParent == this) {
        //GPC_WARNING << "You are trying to create a loop in transform parent" << ENDL;
        return;
    }
    if (mp_Parent != nullptr) {
        RemoveParent();
    }
    mp_Parent = pParent;
    if (mp_Parent != nullptr) {
        mp_Parent->mp_Childs.push_back(this);

        float dx = m_Position.x - mp_Parent->GetPosition().x;
        float dy = m_Position.y - mp_Parent->GetPosition().y;
        m_DistanceFromParent = std::sqrt(dx * dx + dy * dy);

        m_AngleDifferenceToParent = m_RadAngle - mp_Parent->GetRadAngle();

<<<<<<< Updated upstream
        Vector2f offset = m_Position - mp_Parent->GetPosition();
        m_OffsetAngle = MathGC::VecToAngle(offset) - mp_Parent->GetRadAngle();
=======
        m_OffsetAngle = gcle::Math::VecToAngle(m_Position - mp_Parent->GetPosition()) - mp_Parent->GetRadAngle();
>>>>>>> Stashed changes
    }
}

void Transform2D::SetPosition(Vector2f position)
{
    m_Position = position;
}

void Transform2D::SetDirection(Vector2f position)
{
    m_Direction = position;
}

<<<<<<< Updated upstream
void Transform2D::SetDegAngle(Degrees angle)
=======
void Transform2D::SetDegAngle(gcle::Math::Degrees angle)
>>>>>>> Stashed changes
{
    m_DegAngle = angle;
}

<<<<<<< Updated upstream
void Transform2D::SetRadAngle(Radians angle)
=======
void Transform2D::SetRadAngle(gcle::Math::Radians angle)
>>>>>>> Stashed changes
{
    m_RadAngle = angle;
}

void Transform2D::SetDirty() {
    if (m_IsDirty) return;
    m_IsDirty = true;
    for (auto pChild : mp_Childs) {
        pChild->SetDirty();
    }
}

#pragma endregion

#pragma region Add/Remove

void Transform2D::AddChild(Transform2D* pChild) {
    if (pChild == this) {
        //GPC_WARNING << "You are trying to create a loop in transform parent" << ENDL;
        return;
    }
    for (auto& child : mp_Childs) {
        if (child == pChild) return;
    }
    pChild->SetParent(this);

}

void Transform2D::RemoveParent() {
    if (mp_Parent != nullptr) {
        for (auto it = mp_Parent->mp_Childs.begin(); it != mp_Parent->mp_Childs.end(); ++it) {
            if (*it == this) {
                mp_Parent->mp_Childs.erase(it);
                break;
            }
        }
    }
    mp_Parent = nullptr;
}

void Transform2D::RemoveChild(uint32 index) {
    if (index >= GetChildCount()) return;
    auto it = mp_Childs.begin();
    std::advance(it, index);
    mp_Childs[index]->mp_Parent = nullptr;
    mp_Childs.erase(it);
}
#pragma endregion

#pragma region Updates

void Transform2D::RotateToDirection()
{
<<<<<<< Updated upstream
    Radians rad = MathGC::VecToAngle(m_Direction);

    SetRadAngle(rad);
    SetDegAngle(MathGC::RadToDeg(rad));
=======
    gcle::Math::Radians rad = gcle::Math::VecToAngle(m_Direction);

    SetRadAngle(rad);
    SetDegAngle(gcle::Math::RadToDeg(rad));
>>>>>>> Stashed changes
}

void Transform2D::Forward()
{
<<<<<<< Updated upstream
    m_Direction = MathGC::AngleToVec(m_RadAngle);
=======
    m_Direction = gcle::Math::AngleToVec(m_RadAngle);
>>>>>>> Stashed changes
}

void Transform2D::UpdatePositionWithParentPosition()
{
    if (mp_Parent == nullptr) {
        //DEBUG_WARN << "this transform has no parent" << ENDL;
        return;
    }

<<<<<<< Updated upstream
    Radians currentParentAngle = mp_Parent->GetRadAngle();
=======
    gcle::Math::Radians currentParentAngle = mp_Parent->GetRadAngle();
>>>>>>> Stashed changes

    m_Position.x = mp_Parent->GetPosition().x + m_DistanceFromParent * std::cos(currentParentAngle + m_OffsetAngle);
    m_Position.y = mp_Parent->GetPosition().y + m_DistanceFromParent * std::sin(currentParentAngle + m_OffsetAngle);

    m_RadAngle = mp_Parent->GetRadAngle() + m_AngleDifferenceToParent;
<<<<<<< Updated upstream
    m_DegAngle = MathGC::RadToDeg(m_RadAngle);
=======
    m_DegAngle = gcle::Math::RadToDeg(m_RadAngle);
>>>>>>> Stashed changes
}


#pragma endregion