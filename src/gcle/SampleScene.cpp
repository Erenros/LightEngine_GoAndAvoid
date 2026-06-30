#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "AudioEmitter.h"
#include "include.h"
#include "PhysicsManager.h"

void SampleScene::OnInitialize()
{
	Scene::OnInitialize();
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	std::string text = "Test";
	CreateText(text, 40, 40, 20, 20);

	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(0, 0); 
	entity1->Rotate(45);
	entity1->SetRigidBody(true); 
	entity1->SetIsKinematic(true);
	entity1->GetRigidBody().SetDampingStrenght(0.9);
	entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, { 1.0f, 1.0f })->GetShape()->Rotate(45);

	mp_mainCamera->SetFollowing(entity1);

	pSceneCamera = CreateCamera();

	 
	/*Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true); 
	entity2->GetShape()->SetIsKinematic(true);
	entity2->SetTexture("images");

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
	entity4->SetPosition(-200, 100);
	entity4->SetRigidBody(true); 
	entity4->GetShape()->SetIsKinematic(true);*/

	for (int i = 0; i < 500; i++) {
		Entity* entity = CreateEntity<Entity>(gcle::Shapes::Rectangle);
		entity->SetPosition(-49000 + (i * 100), -49000 + (i * 100));
		entity->SetRigidBody(true);
		entity->GetRigidBody().SetGravity(false);
		entity->SetIsKinematic(true);
		entity->CreateCollider(gcle::Shapes::Rectangle, true, { -49000.f + (i * 100), -49000.f + (i * 100) }, { 1, 1 });
	}

	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(true);
	PhysicsManager::GetInstance().SetQuadTreePos1({ -50000, -50000 });
	PhysicsManager::GetInstance().SetQuadTreePos2({ 50000, 50000 });
}

void SampleScene::OnUpdate(Clock& time)
{
	Scene::OnUpdate(time);
	 
	if (InputManager::GetInstance().IsDown('A'))
	{
		if (GetCurrentCamera()->GetId() != mp_mainCamera->GetId())
		{
			SwitchCamera(mp_mainCamera);
		}
		else
		{
			SwitchCamera(pSceneCamera);
		}
	}
} 
