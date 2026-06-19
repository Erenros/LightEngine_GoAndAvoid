#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "GameManager.h"

template <typename S>
Scene* SceneManager::CreateScene(const std::string& tag) {
    static_assert(std::is_base_of<Scene, S>::value, "S must be derived from Scene");

    if (m_Scenes[tag] == nullptr) {
        m_Scenes[tag] = new S();
        Scene* scene = m_Scenes[tag];
        scene->m_tag = tag;
        scene->OnInitialize();
        return m_Scenes[tag];
    }
    std::cerr << "Scene " << tag << "already exists" << std::endl;
}