#include "SampleScene2.h"
#include "Player.h"

void SampleScene2::OnInitialize()
{
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Entity* entity1 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
			entity1->SetPosition(10 * i, -200 * j);
			entity1->SetRigidBody(false);
			entity1->GetRigidBody().SetGravity(false);
			entity1->GetShape()->SetIsKinematic(false);
		}
	}
}

void SampleScene2::OnUpdate(Clock& time)
{
	//pEntity->Rotate(1);

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}
} 
