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
	entity1->SetIsStatic(true); 
	entity1->GetRigidBody().SetDampingStrenght(0.9);
	entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 45.0f, { 1.0f, 1.0f }); 

	mp_mainCamera->SetFollowing(entity1);

	pSceneCamera = CreateCamera();


	for (int32 i = 0; i < 500; i++) {
		Entity* entity = CreateEntity<Entity>(gcle::Shapes::Rectangle);
		entity->SetPosition(-49000.0f + (i * 100.0f), -49000.0f + (i * 100.0f));
		entity->SetRigidBody(true);
		entity->GetRigidBody().SetGravity(false);
		entity->SetIsStatic(true);
		entity->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0,  { 1, 1 });
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
