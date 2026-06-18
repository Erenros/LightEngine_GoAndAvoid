#include "SDL.h"
#include "GameManager.h"
#include "include.h"

#include "SampleScene.h"

#undef main

int main(int argc, char** argv)
{ 
    GameManager gm = GameManager(1000, 800);

    gm.Init();
    gm.LaunchScene<SampleScene>();
    gm.Loop();
    gm.Close();

    return 0;
}