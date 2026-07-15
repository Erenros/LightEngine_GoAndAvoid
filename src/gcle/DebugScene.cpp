#include "DebugScene.h"
#include "DebugPlayer.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Engine/Collider.h"
#include "Engine/PhysicsManager.h"
#include "Tag.h"
//#include "Render/Shape.h"

void DebugScene::OnInitialize()
{
	Scene::OnInitialize();

	std::string text = "Test";
	CreateText(text, { 40, 40 }, 50);

	Collider* setUpCollider = nullptr;

	//creation d'un player classique
	Entity* entity1 = CreateEntity<DebugPlayer>(gcle::Shapes::Rectangle);
	{
		entity1->SetPosition(50, -200);
		entity1->SetRotation(0);

		//tag du player
		entity1->SetTag(Demo::Tag::Player);

		entity1->SetRigidBody(true);
		entity1->GetRigidBody()->SetGravity(10, true);
		entity1->GetRigidBody()->SetMaxSpeed(100);

		// appeler Brake() quand vous le souhaitez pour qu'il fasse effet (au contacte du sol en generale)
		entity1->GetRigidBody()->SetBrakeDeceleration(100);

		//si l'entity est un static elle ne peut pas bouger et coupe la velocité quand un objet 
		//entre en contacte avec (dans la direction de la normal entre eux)
		entity1->SetStatic(false);

		//permet a l'objet de ne pas clip a travers les autres entité 
		//(mais est assez couteux donc a ne mettre que sur un player)
		entity1->GetRigidBody()->SetCollisionOnContinuous();

		//creation d'un collider parametres : (
		// forme, 
		// si il est actif, 
		// position relatif a l'entité, 
		// rotation relative a l'entité, 
		// scale relatif a l'entité
		// )
		setUpCollider = entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0, { 1, 1 });
	}

	//creation d'un trigger
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	{
		entity2->SetPosition(200, -100);
		entity2->SetScale({ 1, 5 });
		entity2->SetRotation(0);
		entity2->SetTag(Demo::Tag::Trigger);
		entity2->SetRigidBody(false);
		entity2->SetStatic(true);
		setUpCollider = entity2->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0, { 1, 1 });
	}

	//creation d'un sol
	Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	{
		entity3->SetPosition(0, 300);
		entity3->SetScale({ 10, 1 });
		entity3->SetTag(Demo::Tag::Ground);
		entity3->SetRotation(0);
		entity3->SetRigidBody(true);
		entity3->SetStatic(true);
		setUpCollider = entity3->CreateCollider(gcle::Shapes::Rectangle, true, { 0, 0 }, 0, { 1, 1 });
	}

	Button* ui = CreateButton(gcle::Shapes::Rectangle, "hey");
	ui->SetScale({ 2, 2 });
	ui->GetText()->SetFontSize(60);
	ui->GetText()->SetColor(0, 0, 0, 255);
	ui->SetPosition(150, 150);
	//ui->SetFunctions({});
	//ou 
	//ui->AddFunction();

	mp_MainCamera->SetFollowing(entity1);

	//pSceneCamera = CreateCamera();

	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(true);
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
