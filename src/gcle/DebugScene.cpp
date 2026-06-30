#include "DebugScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Engine/Collider.h"
#include "Engine/PhysicsManager.h"
//#include "Render/Shape.h"

void DebugScene::OnInitialize()
{
	Scene::OnInitialize();

	std::string text = "Test";
	CreateText(text, 40, 40, 20, 20);

	Collider* setUpCollider = nullptr;

	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(50, -200);
	entity1->SetRotation(0);
	entity1->SetRigidBody(true);
	entity1->GetRigidBody().SetGravity(1000, true);
	entity1->GetRigidBody().SetMaxSpeed(10000);
	entity1->SetIsKinematic(true);
	setUpCollider = entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0, {1, 1});

	mp_mainCamera->SetFollowing(entity1);

	pSceneCamera = CreateCamera();

	for (int i = 0; i <= 20; i++)  {
		Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
		entity3->SetPosition(-700 + i * 100, 300);
		entity3->SetScale({ 1, 1 });
		entity3->SetRigidBody(true);
		entity3->GetRigidBody().SetGravity(false);
		entity3->SetIsKinematic(false);
		setUpCollider = entity3->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0, { 1, 1 });
	}


	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(false);
	PhysicsManager::GetInstance().SetQuadTreePos1({ -50000, -50000 });
	PhysicsManager::GetInstance().SetQuadTreePos2({ 50000, 50000 });
}

void DebugScene::OnUpdate(Clock& time)
{

	Scene::OnUpdate(time);

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}
}
