#include "GameManager.h"
#include "include.h"

#include "SampleScene.h"
#include "SceneManager.h"
#include "Profiler.h"

#undef main

int main(int argc, char** argv)
{ 
    GameManager& gm = GameManager::GetInstance();

    gm.Init(640, 480);

    SceneManager::GetInstance().CreateScene<SampleScene>("SampleScene");
    SceneManager::GetInstance().CreateScene<SampleScene>("SampleScene2");

    SceneManager::GetInstance().SetCurrentSceneWithTag("SampleScene");
    SceneManager::GetInstance().SetCurrentSceneWithTag("SampleScene2");

    
    gm.Loop();
    gm.Close();

    return 0;
}