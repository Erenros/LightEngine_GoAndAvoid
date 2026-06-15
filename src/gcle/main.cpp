#include "SDL.h"
#include "GameManager.h"

#undef main

int main(int argc, char** argv)
{
    GameManager gm = GameManager(640,480);

    gm.LaunchGame();

    ///* Initialisation simple */
    //if (SDL_Init(SDL_INIT_VIDEO) != 0)
    //{ 
    //    return -1;
    //}

    //{
    //    /* Création de la fenêtre */
    //    SDL_Window* pWindow = NULL;
    //    pWindow = SDL_CreateWindow("Ma première application SDL2", SDL_WINDOWPOS_UNDEFINED,
    //        SDL_WINDOWPOS_UNDEFINED,
    //        640,
    //        480,
    //        SDL_WINDOW_SHOWN);

    //    if (pWindow)
    //    {
    //        SDL_Delay(3000); /* Attendre trois secondes, que l'utilisateur voit la fenêtre */

    //        SDL_DestroyWindow(pWindow);
    //    }
    //    else
    //    { 
    //    }
    //}

    //SDL_Quit();

    return 0;
}