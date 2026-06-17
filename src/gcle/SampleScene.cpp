#include "SampleScene.h"

void SampleScene::OnInitialize()
{
	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	Rectangle* rectangle = new Rectangle(0, 0, 300, 300, { 250,250, 250, 250 });


	rectangle->SetTexture(tex);


	Triangle* triangle = new Triangle(0.f, 0.f, 500.f, 300.f, 500.f, 500.f, { 255, 255, 255, 255 });
	triangle->SetTexture(tex);



	Circle* circle = new Circle(0, 0, 100, 50, { 0, 0, 230, 255 });

	pEntity = CreateEntity<Entity>(*circle);
}

void SampleScene::OnUpdate()
{
}
