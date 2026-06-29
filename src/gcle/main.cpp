#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "GameManager.h"
#include "include.h"

#include "SampleScene.h"
#include "SampleScene2.h"
#include "SceneManager.h"
#include "Profiler.h"

#undef main


int main(int argc, char** argv)
{ 
    GameManager& gm = GameManager::GetInstance();

    gm.Init(640, 480, 120);

    SceneManager::GetInstance().CreateScene<SampleScene>("SampleScene");
    //SceneManager::GetInstance().CreateScene<SampleScene2>("SampleScene2");

    SceneManager::GetInstance().SetCurrentSceneWithTag("SampleScene");
    //SceneManager::GetInstance().SetCurrentSceneWithTag("SampleScene2");

    
    gm.Loop();
    gm.Close();

    _CrtDumpMemoryLeaks();

    return 0;
}