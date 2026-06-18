#include "SampleScene.h"
#include "Player.h"
void SampleScene::OnInitialize()
{
	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	gcle::Rectangle* rectangle = new gcle::Rectangle(0, 0, 300, 300, { 250,250, 250, 250 });
	rectangle->SetTexture(tex);
	Entity* entity = CreateEntity<Player>(*rectangle);
	
}

void SampleScene::OnUpdate(Timer& time){
	//DEBUG_INFO << m_tag << ENDL;
}
