#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "AudioEmitter.h"

void SampleScene::OnInitialize()
{
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	std::string text = "Test";
	CreateText(text, 40, 40, 20, 20);
	 
	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity1->SetPosition(10, -200);
	entity1->SetRotation(45);
	entity1->SetRigidBody(true); 
	entity1->GetShape()->SetIsKinematic(true);
	entity1->GetRigidBody().SetDampingStrenght(0.9);

	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true); 
	entity2->GetShape()->SetIsKinematic(true);
	entity2->SetTexture("images");

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


	if (InputManager::GetInstance().IsControllerDown(XBOX_A))
	{
		std::cout << "XBOX A" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_B))
	{
		std::cout << "XBOX B" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_X))
	{
		std::cout << "XBOX X" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_Y))
	{
		std::cout << "XBOX Y" << std::endl;
	}





	if (InputManager::GetInstance().IsControllerDown(XBOX_BACK))
	{
		std::cout << "XBOX BACK" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_PAD_LEFT))
	{
		std::cout << "XBOX PAD LEFT" << std::endl;
	}





	if (InputManager::GetInstance().IsControllerDown(XBOX_PAD_RIGHT))
	{
		std::cout << "XBOX PAD RIGHT" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_PAD_UP))
	{
		std::cout << "XBOX PAD UP" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_PAD_DOWN))
	{
		std::cout << "XBOX PAD DOWN" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_PAD_DOWN))
	{
		std::cout << "XBOX PAD DOWN" << std::endl;
	}




	if (InputManager::GetInstance().IsControllerDown(XBOX_RJS))
	{
		std::cout << "XBOX RT DOWN" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_LJS))
	{
		std::cout << "XBOX LT DOWN" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_RB))
	{
		std::cout << "XBOX RB DOWN" << std::endl;
	}

	if (InputManager::GetInstance().IsControllerDown(XBOX_LB))
	{
		std::cout << "XBOX LB DOWN" << std::endl;
	}
} 
