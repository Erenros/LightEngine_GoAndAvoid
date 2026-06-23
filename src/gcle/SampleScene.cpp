#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"
#include "Render/Text.h"

void SampleScene::OnInitialize()
{
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);*/
	 
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");

	 
	Entity* entity1 = CreateEntity<Entity>(gcle::Shapes::Circle);
 	entity1->SetPosition(10, -200);
	entity1->SetRigidBody(true);
	entity1->GetShape()->SetIsKinematic(true);
	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->GetShape()->SetIsKinematic(false);
	entity2->SetTexture("images");
}

void SampleScene::OnUpdate(Clock& time)
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
