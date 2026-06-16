//#include "SceneManager.h"
//#include "Scene.h"
//
//
//static SceneManager* mp_Instance = nullptr;
//
//SceneManager* SceneManager::Get()
//{
//    if (mp_Instance == nullptr) {
//        mp_Instance = new SceneManager;
//    }
//    return mp_Instance;
//}
//
//
//Scene* SceneManager::GetCurrentScene(){
//    return m_Scenes[m_CurrentSceneTag];
//}
//
//std::string& SceneManager::GetCurrentSceneTag() {
//    return m_CurrentSceneTag;
//}
//
//
//Scene* SceneManager::GetPreviousScene()
//{
//    return m_Scenes[m_PreviousSceneTag];
//}
//
//std::string& SceneManager::GetPreviousSceneTag(){
//    return m_PreviousSceneTag;
//
//}
//
//
//
//
//
//Scene* SceneManager::GetSceneWithTag(const std::string& tag){
//    return m_Scenes[tag];
//}
//
//
//
//void SceneManager::SetCurrentSceneWithTag(const std::string& tag){
//    if (m_Scenes[tag] != nullptr) {
//        m_PreviousSceneTag = m_CurrentSceneTag;
//        m_CurrentSceneTag = tag;
//        return;
//    }
//    std::cerr << "tag : " << tag << " doesn't exist " << std::endl;
//}
//
//void SceneManager::SetCurrentSceneToPreviousScene(){
//    if (m_PreviousSceneTag != "") {
//        const std::string& currentTag = m_PreviousSceneTag;
//        m_PreviousSceneTag = m_CurrentSceneTag;
//        m_CurrentSceneTag = currentTag;
//        return;
//    }
//    std::cerr << "there is no previous scene " << std::endl;
//}
//
//
//Scene* SceneManager::CreateScene(const std::string& tag){
//
//    if (m_Scenes[tag] == nullptr) {
//        m_Scenes[tag] = new Scene;
//        return m_Scenes[tag];
//    }
//    std::cerr << "Scene " << tag << "already exists" << std::endl;
//}
//
//void SceneManager::DeleteScene(const std::string& tag){
//    {
//        if (m_Scenes[tag] != nullptr) {
//            Scene* scene = m_Scenes[tag];
//            m_Scenes.erase(tag);
//
//            if (m_CurrentSceneTag == tag) {
//                m_CurrentSceneTag = "";
//            }
//            if (m_PreviousSceneTag == tag) {
//                m_PreviousSceneTag = "";
//            }
//
//            delete scene;
//            return;
//        }
//        std::cerr << "Scene " << tag << "doesn't exist" << std::endl;
//
//    }
//}
//
//
//void SceneManager::UpdateCurrentScene(){
//    if (m_CurrentSceneTag != "") {
//        m_Scenes[m_CurrentSceneTag]->Update();
//    }
//}
//
//void SceneManager::DrawCurrentScene(Window* pWindow){
//    if (m_CurrentSceneTag != "") {
//        m_Scenes[m_CurrentSceneTag]->Draw(pWindow);
//    }
//}
//
