#pragma once
#include "Scene.h"

#include "GameManager.h"


template<typename T>
T* Scene::CreateEntity(gcle::Shapes shape)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* new_entity = GCLE_NEW T();
	Entity* entity = new_entity;

	entity->m_activeScenes.push_back(m_tag);

	entity->Initialize(shape);

	GameManager::GetInstance().AddEntity(new_entity);

	return new_entity;
}
