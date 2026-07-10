#include "Table.h"

#include "Tuto-GCLE/Tag.h"

void Demo::Table::OnInitialize()
{
    SetTag(GameTag::Decor);

    ScaleBy({ 0.8, 0.64 });
    CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0, { 1.0f, 1.0f });

    SetRigidBody(true);
    GetRigidBody().SetGravity(false);
    SetStatic(true);
    GetRigidBody().SetCollisionOnContinuous();

    SetTexture("Table_Vertical");
}

void Demo::Table::OnInteract(Entity* pEntity)
{
    if (m_IsDown == false)
    {
        SetTag(GameTag::Wall);

        SetStatic(false);
        GetRigidBody().ActivateDamping(true);
        GetRigidBody().SetDampingStrenght(0.99f);

        m_CanInteract = false;
        m_IsDown = true;
    } 
}

bool Demo::Table::IsDown() const
{
    return m_IsDown;
}
