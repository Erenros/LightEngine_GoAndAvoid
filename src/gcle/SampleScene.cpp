#include "SampleScene.h"
#include "Player.h"
#include "Render/Sprite.h"

void SampleScene::OnInitialize()
{
	Texture* tex = RessourceManager::GetInstance().GetTexture("images"); 
	 
	pEntity = CreateEntity<Player>(gcle::Shapes::Rectangle);
	Sprite* sprite = new Sprite(GameManager::GetInstance().GetWindow(), "../../assets/textures/test.png");
	sprite->AddAnimation("test", new Animation(0, 3, 0, 32, 32,1.5));
	sprite->PlayAnimation("test");
	pEntity->SetTexture(sprite);
	//pEntity->SetScale(0.5f);
	//pEntity->SetRigidBody(true);   
	pEntity->GetRigidBody().SetGravity(false);

	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");

	std::string text = "Test";
	CreateText(font,text , 40, 40, 20, 20);
	 
	/*Entity* entity1 = CreateEntity<Entity>(gcle::Shapes::Circle);
	entity1->SetPosition(10, -200);
	entity1->SetRigidBody(true);
	entity1->GetShape()->SetIsKinematic(true);
	 
	Entity* entity2 = CreateEntity<Entity>(gcle::Shapes::Rectangle);
	entity2->SetPosition(10, 100);
	entity2->SetRigidBody(true);
	entity2->GetRigidBody().SetGravity(false);
	entity2->GetShape()->SetIsKinematic(false);*/
}

void SampleScene::OnUpdate(Clock& time)
{
	//pEntity->Rotate(1);
	pEntity->GetId();

	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}

	if (InputManager::GetInstance().IsDown('A'))
	{
		pEntity->Destroy();
	}
} 
