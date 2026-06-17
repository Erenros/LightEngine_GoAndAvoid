#pragma once
#include "Scene.h"

#include "GameManager.h"


template<typename T>
T* Scene::CreateEntity(Shape& shape)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* new_entity = new T();
	Entity* entity = new_entity;

	entity->Initialize(shape);

	mp_GameManager->AddEntity(new_entity);

	return new_entity;
}
