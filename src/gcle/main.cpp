#include "SDL.h"
#include "GameManager.h"
#include "include.h"

#undef main

int main(int argc, char** argv)
{
    //Temporaire
    GameManager* gm = GameManager::Get();

    gm->LaunchGame(500, 500);

    ///* Initialisation simple */
    //if (SDL_Init(SDL_INIT_VIDEO) != 0)
    //{ 
    //    return -1;
    //}

    //{
    //    /* Crï¿½ation de la fenï¿½tre */
    //    SDL_Window* pWindow = NULL;
    //    pWindow = SDL_CreateWindow("Ma premiï¿½re application SDL2", SDL_WINDOWPOS_UNDEFINED,
    //        SDL_WINDOWPOS_UNDEFINED,s
    //        640,
    //        480,
    //        SDL_WINDOW_SHOWN);

    //    if (pWindow)
    //    {
    //        SDL_Delay(3000); /* Attendre trois secondes, que l'utilisateur voit la fenï¿½tre */

    //        SDL_DestroyWindow(pWindow);
    //    }
    //    else
    //    { 
    //    }
    //}

    //SDL_Quit();

    return 0;
}