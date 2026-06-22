#include "SampleScene.h"
#include "Player.h"

void SampleScene::OnInitialize()
{
	Texture* tex = RessourceManager::GetInstance().GetTexture("images");
	 
	pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);   
	 
	Entity* entity1 = CreateEntity<Entity>(gcle::Shapes::Circle);
	entity1->SetPosition(100, 100);
	entity1->SetRigidBody(true);
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Circle);
	entity2->SetPosition(-100, -100);
	entity2->SetRigidBody(true);
	
}

void SampleScene::OnUpdate(Timer& time)
{
	pEntity->Rotate(1);

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}
} 
