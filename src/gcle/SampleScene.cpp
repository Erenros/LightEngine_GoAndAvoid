#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"

void SampleScene::OnInitialize()
{
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	std::string text = "Test";
	CreateText(text, 40, 40, 20, 20);
	 
	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(10, -200);
	entity1->SetRotation(45);
	entity1->SetRigidBody(true);
	entity1->GetRigidBody().SetGravity(false);
	entity1->GetShape()->SetIsKinematic(true);
	entity1->GetRigidBody().SetDampingStrenght(0.9);

	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->GetShape()->SetIsKinematic(false);
	entity2->SetTexture("images");
}

void SampleScene::OnUpdate(Clock& time)
{

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}
} 
