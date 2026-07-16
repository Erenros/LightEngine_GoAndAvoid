#include "DemoScene.h"

#include "Engine/PhysicsManager.h"
#include "Render/Text.h"

#include "Tuto-GCLE/DemoPlayer.h"
#include "Tuto-GCLE/GCEnemy.h"
#include "Tuto-GCLE/Table.h"
#include "Tuto-GCLE/Wall.h"
#include "Tuto-GCLE/Tag.h"

void DemoScene::OnInitialize()
{

	Scene::OnInitialize();
	 

	pSceneCamera = CreateCamera();
	SwitchCamera(pSceneCamera);

	mp_Player = CreateEntity<Demo::GCPlayer>(gcle::Shapes::Rectangle);
	SetSelectedEntity(mp_Player);

	m_Smooth.Initialize(pSceneCamera, mp_Player);

	//CreateEntity<Demo::Table>(gcle::Shapes::Rectangle);

	mp_Enemy = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	mp_Enemy->SetTarget(mp_Player);
	mp_Enemy->SetDetectionRange(600.0f);
	mp_Enemy->SetAttackRange(400.0f);
	mp_Enemy->SetPosition(100.0f, 100.0f);


	//Demo::GCEnemy* pEnemy2 = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	//pEnemy2->SetTarget(mp_Player);
	//pEnemy2->SetPosition(-200.0f, 150.0f);

	//Demo::GCEnemy* pEnemy3 = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	//pEnemy3->SetTarget(mp_Player);
	//pEnemy3->SetPosition(200.0f, -150.0f);



	 
	
	Demo::Wall* pWall = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall->SetPosition(0, -400);
	pWall->SetScale({ 8.0f, 1.0f });

	Demo::Wall* pWall2 = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall2->SetPosition(0, 400);
	pWall2->SetScale({ 8.0f, 1.0f });

	Demo::Wall* pWall3 = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall3->SetPosition(-400, 0);
	pWall3->SetScale({ 1.0f, 8.0f });

	Demo::Wall* pWall4 = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall4->SetPosition(400, 0);
	pWall4->SetScale({ 1.0f, 8.0f }); 

	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(true);
	PhysicsManager::GetInstance().SetQuadTreePos1({ -5000, -5000 });
	PhysicsManager::GetInstance().SetQuadTreePos2({ 5000, 5000 });

}

void DemoScene::OnUpdate(Clock& time)
{
	Scene::OnUpdate(time);

	m_Smooth.Update(static_cast<float>(time.GetDeltaTime())); 

	if (InputManager::GetInstance().IsDown('O'))
		SceneManager::GetInstance().SetCurrentSceneWithTag("Sample", true);

}
