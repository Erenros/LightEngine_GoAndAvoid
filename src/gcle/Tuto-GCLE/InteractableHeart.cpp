#include "InteractableHeart.h"
#include "Tuto-GCLE/Tag.h"
#include "Tuto-GCLE/DemoPlayer.h"

void Demo::InteractableHeart::OnInitialize()
{
    SetTag(GameTag::Decor);

    //ScaleBy({ 0.8, 0.64 });
    CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0, { 1.0f, 1.0f }, true);

    SetRigidBody(true);
    GetRigidBody().SetGravity(false);
    SetStatic(true); 

    SetTexture("DropHeart");
}



void Demo::InteractableHeart::OnInteract(Entity* pEntity)
{
    GCPlayer* pPlayer = static_cast<GCPlayer*>(pEntity);
    pPlayer->Heal(m_HealAmount);
    Destroy();
}

void Demo::InteractableHeart::OnUpdate()
{
    if (CanBeInteractWith())
    {
        SetTexture("InteractDropHeart");
    }
    else
    {
        SetTexture("DropHeart");
    }
}
