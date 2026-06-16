#pragma once
#include <vector>

class Entity;
class Window;


class Scene{
	std::vector<Entity*> m_Entities;


	void Update();
	void Draw(Window* pWindow);


	//Constructors 

	Scene() = default;

	//Destructors

	~Scene() = default;


	friend class SceneManager;


public:
	void AddEntity(Entity* entity);
	Entity* GetEntityWithID(const int& id);

	/*template <typename T, typename... Args>
	Entity* CreateEntity(Args&&... args) {
		static_assert(std::is_base_of<Entity, T>::value, "T must inherit from Entity");
		std::cerr << "T must inherit from Entity" << std::end;
		T* entity = new T(std::forward<Args>(args)...);

		m_Entities.push_back(entity);
		return entity;
	}*/
};

