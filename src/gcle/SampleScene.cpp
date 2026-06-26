#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "AudioEmitter.h"
#include "include.h"
#include "PhysicsManager.h"

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
	entity1->GetShape()->SetIsKinematic(true);
	entity1->GetRigidBody().SetDampingStrenght(0.9f);


	/*Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity3->SetPosition(200.f, 100.f);
	entity3->SetRotation(45.f);
	entity3->SetRigidBody(true);
	entity3->GetRigidBody().SetGravity(false);
	entity3->GetShape()->SetIsKinematic(true);
	entity3->SetTexture("images");
	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10.f, 100.f);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->GetShape()->SetIsKinematic(true);
	entity2->SetTexture("images");

	Entity* entity4 = CreateEntity<Entity>(gcle::Shapes::Circle);
	entity4->SetPosition(-200.f, 100.f);
	entity4->SetRigidBody(true);
	entity4->GetRigidBody().SetGravity(false);
	entity4->GetShape()->SetIsKinematic(true);*/

	for (int i = 0; i < 200; i++) {
		Entity* entity = CreateEntity<Entity>(gcle::Shapes::Rectangle);
		entity->SetPosition(-0000 + (i * 100), -0000 + (i * 100));
		entity->SetRigidBody(true);
		entity->GetRigidBody().SetGravity(false);
		entity->GetShape()->SetIsKinematic(true);
	}

	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(true);
	PhysicsManager::GetInstance().SetQuadTreePos1({-50000, -50000});
	PhysicsManager::GetInstance().SetQuadTreePos2({50000, 50000});
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
