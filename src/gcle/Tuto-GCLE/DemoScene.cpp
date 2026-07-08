#include "DemoScene.h"

#include "Engine/PhysicsManager.h"

#include "Tuto-GCLE/DemoPlayer.h"
#include "Tuto-GCLE/GCEnemy.h"
#include "Tuto-GCLE/Tag.h"

void DemoScene::OnInitialize()
{
	Scene::OnInitialize();

	pSceneCamera = CreateCamera();
	SwitchCamera(pSceneCamera);

	mp_Player = CreateEntity<Demo::GCPlayer>(gcle::Shapes::Rectangle);

	mp_Enemy = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	mp_Enemy->SetTarget(mp_Player);
	mp_Enemy->SetDetectionRange(600.0f);
	mp_Enemy->SetAttackRange(400.0f);
	mp_Enemy->SetPosition(100.0f, 100.0f);

	Demo::GCEnemy* pEnemy2 = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	pEnemy2->SetTarget(mp_Player);
	pEnemy2->SetPosition(-200.0f, 150.0f);

	Demo::GCEnemy* pEnemy3 = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	pEnemy3->SetTarget(mp_Player);
	pEnemy3->SetPosition(200.0f, -150.0f);



	Entity* pWall1 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	pWall1->SetPosition(0, -400);
	pWall1->SetScale({8.0f, 1.0f});
	pWall1->SetTag(Demo::GameTag::Wall);
	pWall1->SetRigidBody(true);
	pWall1->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
	pWall1->SetStatic(true);

	Entity* pWall2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	pWall2->SetPosition(0, 400);
	pWall2->SetScale({ 8.0f, 1.0f });
	pWall2->SetTag(Demo::GameTag::Wall);
	pWall2->SetRigidBody(true);
	pWall2->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
	pWall2->SetStatic(true);

	Entity* pWall3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	pWall3->SetPosition(-400, 0);
	pWall3->SetScale({ 1.0f, 8.0f });
	pWall3->SetTag(Demo::GameTag::Wall);
	pWall3->SetRigidBody(true);
	pWall3->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
	pWall3->SetStatic(true);

	Entity* pWall4 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	pWall4->SetPosition(400, 0);
	pWall4->SetScale({ 1.0f, 8.0f });
	pWall4->SetTag(Demo::GameTag::Wall);
	pWall4->SetRigidBody(true);
	pWall4->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
	pWall4->SetStatic(true);

	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(true);
	PhysicsManager::GetInstance().SetQuadTreePos1({ -5000, -5000 });
	PhysicsManager::GetInstance().SetQuadTreePos2({ 5000, 5000 });

}

void DemoScene::OnUpdate(Clock& time)
{
	Scene::OnUpdate(time);
}
