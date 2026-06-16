#include "SDL.h"
#include "GameManager.h"

#undef main

int main(int argc, char** argv)
{
    //Temporaire
    GameManager gm = GameManager(640,480);

    gm.LaunchGame();

    ///* Initialisation simple */
    //if (SDL_Init(SDL_INIT_VIDEO) != 0)
    //{ 
    //    return -1;
    //}

    //{
    //    /* Cr�ation de la fen�tre */
    //    SDL_Window* pWindow = NULL;
    //    pWindow = SDL_CreateWindow("Ma premi�re application SDL2", SDL_WINDOWPOS_UNDEFINED,
    //        SDL_WINDOWPOS_UNDEFINED,
    //        640,
    //        480,
    //        SDL_WINDOW_SHOWN);

    //    if (pWindow)
    //    {
    //        SDL_Delay(3000); /* Attendre trois secondes, que l'utilisateur voit la fen�tre */

    //        SDL_DestroyWindow(pWindow);
    //    }
    //    else
    //    { 
    //    }
    //}

    //SDL_Quit();

    return 0;
}