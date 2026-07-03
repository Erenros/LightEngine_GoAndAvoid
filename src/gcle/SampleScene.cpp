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
	 
	 
	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Circle);
	entity1->SetPosition(0, 0);
	entity1->SetRotation(0);
	entity1->SetRigidBody(true); 
	entity1->GetShape()->SetIsKinematic(true);
	entity1->GetRigidBody().SetDampingStrenght(0.9f);
	//entity1->SetTexture("images");
	entity1->SetTexture("test");
;
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
