#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "GameManager.h"
#include "PhysicsManager.h"
#include "AssetEngine.h"

#include "include.h"

#include "DebugScene.h"
#include "SampleScene2.h"
#include "SampleScene.h"
#include "SceneManager.h"
#include "Profiler.h"

#include "Tuto-GCLE/DemoScene.h"

#undef main

#include <windows.h>

#ifdef _DEBUG
#include <cstdio>
#include <iostream>

void CreateDebugConsole()
{
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio(true);

    SetConsoleTitleA("GCLE Debug Console");
}
#endif

void CreateScenes()
{
    SceneManager::GetInstance().CreateScene<DemoScene>("DebugScene");
    SceneManager::GetInstance().CreateScene<DebugScene>("Sample");
}

void Destroy()
{
    GameManager::DestroyInstance();     
    SceneManager::DestroyInstance();
    PhysicsManager::DestroyInstance();  
    RessourceManager::DestroyInstance();
    AssetEngine::DestroyInstance();
    InputManager::DestroyInstance();
} 

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
    CreateDebugConsole();
#endif 

    //_CrtSetBreakAlloc(29302);

    GameManager& gm = GameManager::GetInstance();


    gm.Init(640, 480, 120);
     
    CreateScenes();
    SceneManager::GetInstance().SetCurrentSceneWithTag("DebugScene");
     
    gm.Loop();

    gm.Close();

    Destroy();

    _CrtDumpMemoryLeaks();

    return 0;
}