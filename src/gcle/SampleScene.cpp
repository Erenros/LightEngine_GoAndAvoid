#include "SampleScene.h"
#include "Player.h"

void SampleScene::OnInitialize()
{
	Texture* tex = RessourceManager::GetInstance().GetTexture("images"); 
	 
	/*pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	pEntity->SetTexture(tex);
	pEntity->SetScale(0.5f);
	pEntity->SetRigidBody(true);   */
	 
	

	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");

	std::string text = "Test";
	CreateText(font, text, 40, 40, 20, 20);
	 
	Entity* entity1 = CreateEntity<Player>(gcle::Shapes::Circle);
	entity1->SetPosition(10, -200);
	entity1->SetRigidBody(true);
	entity1->GetRigidBody().SetGravity(true);
	entity1->GetShape()->SetIsKinematic(true);
	 
	Entity* entity2 = CreateEntity<Player>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->GetShape()->SetIsKinematic(true);
	 
	Entity* entity3 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity3->SetPosition(320, 300);
	entity3->SetScale({ 10, 1 });
	entity3->SetRigidBody(true);
	entity3->GetRigidBody().SetGravity(false);
	entity3->GetShape()->SetIsKinematic(false);
	
}

void SampleScene::OnUpdate(Clock& time)
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
