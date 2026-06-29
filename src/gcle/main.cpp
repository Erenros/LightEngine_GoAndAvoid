#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "GameManager.h"
#include "include.h"

#include "DebugScene.h"
#include "SampleScene2.h"
#include "SceneManager.h"
#include "Profiler.h"

#undef main


int main(int argc, char** argv)
{ 
    GameManager& gm = GameManager::GetInstance();

    gm.Init(640, 480);

    SceneManager::GetInstance().CreateScene<DebugScene>("DebugScene");

    SceneManager::GetInstance().SetCurrentSceneWithTag("DebugScene");

    
    gm.Loop();
    gm.Close();

    _CrtDumpMemoryLeaks();

    return 0;
}