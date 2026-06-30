#include "DebugScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Engine/Collider.h"
//#include "Render/Shape.h"

void DebugScene::OnInitialize()
{

	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/

	std::string text = "Test";
	CreateText(text, 40, 40, 20, 20);

	Collider* setUpCollider = nullptr;

	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(50, -200);
	entity1->SetRotation(45);
	entity1->SetRigidBody(true);
	entity1->GetRigidBody().SetGravity(false);
	entity1->SetIsKinematic(true);
	setUpCollider = entity1->CreateCollider(gcle::Shapes::Rectangle, true, { 50, -200 }, 0, {1, 1});
	setUpCollider->GetShape()->SetRotation(45);
	

	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(320, 150);
	entity2->SetScale({ 2, 1 });
	entity2->Rotate(180);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	setUpCollider = entity2->CreateCollider(gcle::Shapes::Rectangle, true, { 320, 150 }, 0, { 2, 1 });
	setUpCollider->GetShape()->SetRotation(0);
	entity2->SetIsKinematic(true);

	Entity* entity4 = CreateEntity<Entity>(gcle::Shapes::Circle);
	entity4->SetPosition(20, 150);
	entity4->SetScale({ 1, 1 });
	entity4->SetRigidBody(true);
	entity4->GetRigidBody().SetGravity(false);
	entity4->SetIsKinematic(true);
	setUpCollider = entity4->CreateCollider(gcle::Shapes::Circle, true, { 20, 150 }, 0, { 1, 1 });

	Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity3->SetPosition(320, 300);
	entity3->SetScale({ 10, 1 });
	entity3->SetRigidBody(true);
	entity3->GetRigidBody().SetGravity(false);
	entity3->SetIsKinematic(false);
	setUpCollider = entity3->CreateCollider(gcle::Shapes::Rectangle, true, { 320, 300 }, 0, { 10, 1 });


}

void DebugScene::OnUpdate(Clock& time)
{

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}
}
