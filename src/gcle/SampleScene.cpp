#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "InputManager.h"
#include "AudioEmitter.h"

void SampleScene::OnInitialize()
{
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	std::string text = "Test";
	test = CreateText(text, 40, 40, 100, 60);
	 
	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(10, -200);
	entity1->SetRotation(45);
	entity1->SetRigidBody(true); 
	entity1->GetShape()->SetIsKinematic(true);
	entity1->GetRigidBody().SetDampingStrenght(0.9f);
	entity1->SetTexture("test");

	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true); 
	entity2->GetShape()->SetIsKinematic(true);
	entity2->SetTexture("Test");

	Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity3->SetPosition(200, 100);
	entity3->SetRotation(45);
	entity3->SetRigidBody(true); 
	entity3->GetShape()->SetIsKinematic(true);
	entity3->SetTexture("images");

	Entity* entity4 = CreateEntity<Entity>(gcle::Shapes::Circle);
	entity4->SetPosition(-200, 100);
	entity4->SetRigidBody(true); 
	entity4->GetShape()->SetIsKinematic(true);
}

void SampleScene::OnUpdate(Clock& time)
{



	if (InputManager::GetInstance().IsDown(Space))
	{
		test->SetText("BONJOUR");
	}
	if (InputManager::GetInstance().IsDown('A'))
	{
		test->SetText("Test");
	}

	//if (InputManager::GetInstance().IsDown('A'))
	//{
	//	pEntity->Destroy();
	//}
} 
