#include "SceneManager.h"
#include "Scene.h"


SceneManager& SceneManager::GetInstance(){
     static SceneManager instance;
     return instance;
}


SceneManager::SceneManager()
{
   
}

SceneManager::~SceneManager()
{
    DeleteAllScenes();
}

Scene* SceneManager::GetCurrentScene() {
    return m_Scenes[m_CurrentSceneTag];
}

std::string& SceneManager::GetCurrentSceneTag() {
    return m_CurrentSceneTag;
}


Scene* SceneManager::GetPreviousScene()
{
    return m_Scenes[m_PreviousSceneTag];
}

std::string& SceneManager::GetPreviousSceneTag() {
    return m_PreviousSceneTag;

}

Scene* SceneManager::GetSceneWithTag(const std::string& tag) {
    return m_Scenes[tag];
}

void SceneManager::SetCurrentSceneWithTag(const std::string& tag) {
    if (m_Scenes[tag] != nullptr) {
        if (m_CurrentSceneTag != "") {
            m_PreviousSceneTag = m_CurrentSceneTag;
            LoadUnloadActiveTextures(tag);
        }
        m_CurrentSceneTag = tag;
        return;
    }
    std::cerr << "tag : " << tag << " doesn't exist " << std::endl;
}

void SceneManager::SetCurrentSceneToPreviousScene() {
    if (m_PreviousSceneTag != "") {
        if (m_CurrentSceneTag != "") {
            m_Scenes[m_CurrentSceneTag]->OnExit();
        }     
        LoadUnloadActiveTextures(m_PreviousSceneTag);
        std::string currentTag = m_PreviousSceneTag;
        m_PreviousSceneTag = m_CurrentSceneTag;
        m_CurrentSceneTag = currentTag;
        return;
    }
    std::cerr << "there is no previous scene " << std::endl;
}

void SceneManager::DeleteScene(const std::string& tag) {
    
    if (m_Scenes[tag] != nullptr) {
        Scene* scene = m_Scenes[tag];

        m_Scenes.erase(tag);

        if (m_CurrentSceneTag == tag) {
            m_CurrentSceneTag = "";
        }
        if (m_PreviousSceneTag == tag) {
            m_PreviousSceneTag = "";
        }

        delete scene;
        return;
    }
    std::cerr << "Scene " << tag << "doesn't exist" << std::endl;
}


void SceneManager::UpdateCurrentScene(Clock& time) {
    if (m_CurrentSceneTag != "") {
        //GCLE_INFO << m_CurrentSceneTag << ENDL;

        Scene* scene = m_Scenes[m_CurrentSceneTag];
        scene->Update(time);
        scene->OnUpdate(time);
    }
}

void SceneManager::DrawCurrentScene(Window* pWindow) {
    if (m_CurrentSceneTag != "") {
        m_Scenes[m_CurrentSceneTag]->Draw(pWindow);
    }
}

void SceneManager::DrawCurrentSceneDebug(Window* pWindow)
{
#ifdef _DEBUG
    if (m_CurrentSceneTag != "") {
        m_Scenes[m_CurrentSceneTag]->DrawDebug(pWindow);
    }
#endif // _DEBUG
}

void SceneManager::DeleteAllScenes()
{
    for (auto& scene : m_Scenes)
    {
        delete scene.second;
        scene.second = nullptr;
    }
    m_Scenes.clear();
    m_CurrentSceneTag = "";
    m_PreviousSceneTag = "";
}

void SceneManager::LoadUnloadActiveTextures(const std::string& newSceneId){
    Scene* currentScene = m_Scenes[m_CurrentSceneTag];
    Scene* newScene = m_Scenes[newSceneId];
    std::vector<std::string> m_activatedTextures;

    for (auto& texture : currentScene->m_ActiveTextures) {
        if (std::find(newScene->m_ActiveTextures.begin(), newScene->m_ActiveTextures.end(), texture) != newScene->m_ActiveTextures.end()) {
            m_activatedTextures.push_back(texture); 
            continue;
        }
        else {
            RessourceManager::GetInstance().m_surfaceMap[texture].UnloadTexture();
            GCLE_INFO << "unload " << texture << ENDL;
        }
    }
    for (auto& texture : newScene->m_ActiveTextures) {
        if (std::find(m_activatedTextures.begin(),m_activatedTextures.end(), texture) == m_activatedTextures.end()) {
            std::string path = "../../assets/textures/" + texture + ".png";
            RessourceManager::GetInstance().LoadSurface(GameManager::GetInstance().GetWindow(), path, texture);
            m_activatedTextures.push_back(texture);
            GCLE_INFO << "load " << texture << ENDL;
        }
    }
}

