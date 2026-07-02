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
	 	 
	std::string text = "Test";
	CreateText(text, { 40, 40 }, 50);

	Entity* entity1 = CreateWorldText("Test", 50, "Hack-Regular");
	entity1->SetPosition(-200, 0); 
	//entity1->Rotate(45);
	entity1->SetRigidBody(true); 
	entity1->SetIsKinematic(true); 
	entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 0.f, 0.f }, 0.f, { 1.f, 1.f });

	Entity* entity2 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity2->SetPosition(0, 0); 
	entity2->Rotate(45);
	entity2->SetRigidBody(true); 
	entity2->SetIsKinematic(true); 
	entity2->SetScale(3.f);
	entity2->GetRigidBody().SetDampingStrenght(0.9);
	entity2->CreateCollider(gcle::Shapes::Rectangle, true ,{ 0.f, 0.f }, 45.f, { 2.f, 2.f });
	entity2->SetTexture("test");
	entity2->AddAnimation("bounce", 0, 3, 0, 32, 28, 0.2);
	entity2->PlayAnimation("bounce");
	//entity2->AddFunctionInFrame("bounce", 1, Test);
	//entity2


	mp_mainCamera->SetFollowing(entity2);

	pSceneCamera = CreateCamera();

	//for (int32 i = 0; i < 500; i++) {
	//	Entity* entity = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	//	entity->SetPosition(-49000.0f + (i * 100.0f), -49000.0f + (i * 100.0f));
	//	entity->SetRigidBody(true);
	//	entity->GetRigidBody().SetGravity(false);
	//	entity->SetIsKinematic(true);
	//	entity->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0,  { 1, 1 });
	//}

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
