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
	InputManager IM = InputManager::GetInstance();


	Vector2f LS = IM.LeftStickPressed();
	Vector2f RS = IM.RightStickPressed();

	if ((LS.x != 0) and (LS.y != 0))
	{
		std::cout << "Orientation of left joystick is : " << std::endl << LS.x << std::endl << LS.y << std::endl;
	}

	if ((RS.x != 0) and (RS.y != 0))
	{
		std::cout << "Orientation of right joystick is : " << std::endl << RS.x << std::endl << RS.y << std::endl;
	}

	float LT = IM.LeftTriggerPressed();
	float RT = IM.RightTriggerPressed();


	if (LT > 0)
	{
		std::cout << "Push on left trigger is : " << IM.LeftTriggerPressed() << std::endl;
	}

	if(RT > 0)
	{
		std::cout << "Push on right trigger is : " << IM.RightTriggerPressed() << std::endl;
	}



	if (IM.IsControllerDown(XBOX_A))
	{
		std::cout << "XBOX A is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_B))
	{
		std::cout << "XBOX B is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_X))
	{
		std::cout << "XBOX X is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_Y))
	{
		std::cout << "XBOX Y is down" << std::endl;
	}





	if (IM.IsControllerDown(XBOX_BACK))
	{
		std::cout << "XBOX BACK is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_START))
	{
		std::cout << "XBOX START is down" << std::endl;
	}
	
	
	
	if (IM.IsControllerDown(XBOX_PAD_LEFT))
	{
		std::cout << "XBOX PAD LEFT is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_PAD_RIGHT))
	{
		std::cout << "XBOX PAD RIGHT is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_PAD_UP))
	{
		std::cout << "XBOX PAD UP is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_PAD_DOWN))
	{
		std::cout << "XBOX PAD DOWN is down" << std::endl;
	}

	




	if (IM.IsControllerDown(XBOX_RJS))
	{
		std::cout << "XBOX RIGHT JOYSTICK is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_LJS))
	{
		std::cout << "XBOX LEFT JOYSTICK is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_RB))
	{
		std::cout << "XBOX RB is down" << std::endl;
	}

	if (IM.IsControllerDown(XBOX_LB))
	{
		std::cout << "XBOX LB is down" << std::endl;

		IM.SetVibration(1, 1);
	}

	else
	{
		IM.SetVibration(0, 0);
	}
} 
