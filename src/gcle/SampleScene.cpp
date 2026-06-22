#include "SampleScene.h"
#include "Player.h"

void SampleScene::OnInitialize()
{
	Texture* tex = RessourceManager::GetInstance().GetTexture("images");
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");

	std::string text = "Test";
	CreateText(font,text , 40, 40, 20, 20);

	gcle::Rectangle* rectangle = new gcle::Rectangle(0, 0, 20, 20, { 250,250, 250, 250 });
	rectangle->SetTexture(tex);
	Entity* entity = CreateEntity<Player>(*rectangle);
	entity->SetRigidBody(true);

	gcle::Rectangle* rectangle2 = new gcle::Rectangle(200, 200, 100, 100, { 250,250, 250, 250 });
	gcle::Rectangle* rectangle3 = new gcle::Rectangle(200, 50, 100, 100, { 250,250, 250, 250 });

	rectangle2->SetTexture(tex);


	gcle::Triangle* triangle = new gcle::Triangle(100.f, 100.f, 600.f, 400.f, 600.f, 600.f, { 255, 0, 0, 255 });
	triangle->SetTexture(tex);



	gcle::Circle* circle = new gcle::Circle(200, 0, 50, 50, { 0, 0, 230, 255 });
	gcle::Circle* circle2 = new gcle::Circle(50, 50, 10, 50, { 0, 0, 230, 255 });
	gcle::Circle* circle3 = new gcle::Circle(100, 100, 10, 50, { 0, 0, 230, 255 });

	/*pEntity = CreateEntity<Entity>(*rectangle);
	pEntity->SetRigidBody(true);*/
	Entity* entity1 = CreateEntity<Entity>(*circle2);
	entity1->SetRigidBody(true);
	Entity* entity2 = CreateEntity<Entity>(*circle3);
	entity2->SetRigidBody(true);
	
}

void SampleScene::OnUpdate(Timer& time)
{
	if (InputManager::GetInstance().IsDown(Space))
	{
		SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
	}
} 
