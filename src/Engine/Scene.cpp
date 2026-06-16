#include "Scene.h"


//TODO : mettre l'update des entitiés
void Scene::Update(){

}

//TODO : mettre le draw des entitiés
void Scene::Draw(Window* pWindow){

}

void Scene::AddEntity(Entity* entity){
	m_Entities.push_back(entity);
}

Entity* Scene::GetEntityWithID(const int& id){
	return m_Entities[id];
}
