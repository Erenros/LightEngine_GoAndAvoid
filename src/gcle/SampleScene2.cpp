#include "SampleScene2.h"
#include "Player.h"

void SampleScene2::OnInitialize()
{
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	

	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	 
	Entity* entity1 = CreateEntity<Entity>(gcle::Shapes::Circle);
 	entity1->SetPosition(10, -200);
	entity1->SetRigidBody(true);
	entity1->SetIsKinematic(true);
	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->SetIsKinematic(false);
	entity2->SetTexture("test");
	entity2->AddAnimation("anim1", 0, 3, 0, 32, 32);
	entity2->PlayAnimation("anim1");
}

void SampleScene2::OnUpdate(Clock& time)
{
	//pEntity->Rotate(1);

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}
} 
