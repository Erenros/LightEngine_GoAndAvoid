#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "GameManager.h"

template <typename S>
Scene* SceneManager::CreateScene(const std::string& tag) {
    if (m_validFlag.size() == 0) {
        std::cerr << "You can't create more than 32 scenes" << std::endl;
        DEBUG_ERROR << "There is no more scene flag disponible" << ENDL;
        return nullptr;
    }


    static_assert(std::is_base_of<Scene, S>::value, "S must be derived from Scene");

    if (m_Scenes[tag] == nullptr) {
        m_Scenes[tag] = new S();
        Scene* scene = m_Scenes[tag];
        scene->m_tag = tag;
        if (m_CurrentSceneTag == "") {
            SetCurrentSceneWithTag(tag);
        }
        scene->OnInitialize();

        int8 id = m_validFlag[0];
        scene->m_flag |= (1 << id);
        m_validFlag.erase(m_validFlag.begin());
        

        return m_Scenes[tag];
    }
    std::cerr << "Scene " << tag << "already exists" << std::endl;
    return nullptr;
}