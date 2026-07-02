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
	CreateText(text, { 40, 40 }, 50);

	pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetPosition(0, 0); 
	pEntity->Rotate(0);
	pEntity->SetRigidBody(true); 
	pEntity->SetIsKinematic(true); 
	pEntity->GetRigidBody().SetDampingStrenght(0.9);
	pEntity->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }); 

	mp_mainCamera->SetFollowing(pEntity);

	pSceneCamera = CreateCamera();


	for (int32 i = 0; i < 500; i++) {
		Entity* entity = CreateEntity<Entity>(gcle::Shapes::Rectangle);
		entity->SetPosition(-49000.0f + (i * 100.0f), -49000.0f + (i * 100.0f));
		entity->SetRigidBody(true);
		entity->GetRigidBody().SetGravity(false);
		entity->SetIsKinematic(true);
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
