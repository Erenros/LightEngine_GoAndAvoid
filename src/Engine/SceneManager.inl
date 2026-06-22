#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "GameManager.h"

template <typename S>
Scene* SceneManager::CreateScene(const std::string& tag) {
    if (m_sceneCount == 32) {
        std::cerr << "You can't create more than 32 scenes" << std::endl;
        DEBUG_ERROR << "You can't create more than 32 scenes" << ENDL;
        return nullptr;
    }
    static_assert(std::is_base_of<Scene, S>::value, "S must be derived from Scene");

    if (m_Scenes[tag] == nullptr) {
        m_Scenes[tag] = new S();
        Scene* scene = m_Scenes[tag];
        scene->m_tag = tag;
        scene->OnInitialize();

        byte 0 << m_sceneCount;

        scene->m_flag[m_sceneCount] = 1;
        m_sceneCount++;

        return m_Scenes[tag];
    }
    std::cerr << "Scene " << tag << "already exists" << std::endl;
    return nullptr;
}