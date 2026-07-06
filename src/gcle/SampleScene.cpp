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
	 	 
	//std::string text = "Test";
	//CreateText(text, { 40, 40 }, 50);

	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(-200, 0); 
	//entity1->Rotate(45);
	entity1->SetRigidBody(true); 
	entity1->SetStatic(false); 
	entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 0.f, 0.f }, 0.f, { 1.f, 1.f });

	mp_mainCamera->SetFollowing(entity1);

	pSceneCamera = CreateCamera();
	pSceneCamera->SetFollowing(entity1);

	for (int32 i = 0; i < 50; i++) 
	{
		for (int32 y = 0; y < 10; y++)
		{
			Entity* entity = CreateEntity<Entity>(gcle::Shapes::Rectangle);
			entity->SetScale(0.2f);
			entity->SetPosition(0.0f + i * 100, 0.0f + y * 100);
			entity->SetRigidBody(true);
			entity->GetRigidBody().SetGravity(false); 
			entity->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0, { 1.0f, 1.0f });
			entity->SetStatic(true);
		}
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
