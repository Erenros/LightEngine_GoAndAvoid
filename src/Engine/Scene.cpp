#include "Scene.h"



void Scene::Draw(Window* window) {
	for (Entity* e : GameManager::GetInstance().m_entities) {
		if(e->IsActiveIn(m_tag)){
			GameManager::GetInstance().GetWindow()->Draw(e->GetShape());
		}
	}
}

void Scene::Update(Timer& time){
	for (Entity* e : GameManager::GetInstance().m_entities) {
		if (e->IsActiveIn(m_tag)) {
			e->Update(time);
		}
	}
} 

