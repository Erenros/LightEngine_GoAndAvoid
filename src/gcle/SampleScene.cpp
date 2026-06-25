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
	 
	/*std::string text = "Test";
	CreateText(text, 40, 40, 20, 20);
	 
	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(50, -200);
	entity1->SetRigidBody(true);
	entity1->GetRigidBody().SetGravity(true);
	entity1->GetShape()->SetIsKinematic(true);
	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(320, 150);
	entity2->SetScale({ 2, 1 });
	entity2->Rotate(180);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->GetShape()->SetIsKinematic(false);
	 
	Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity3->SetPosition(320, 300);
	entity3->SetScale({ 10, 1 });
	entity3->SetRigidBody(true);
	entity3->GetRigidBody().SetGravity(false);
	entity3->GetShape()->SetIsKinematic(false);*/
	
}

void SampleScene::OnUpdate(Clock& time)
{

	/*if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}*/
} 
