#include "SDL.h"
#include "include.h"

#undef main

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    { 
        return -1;
    }

    {
        SDL_Window* pWindow = NULL;
        pWindow = SDL_CreateWindow("Ma première application SDL2", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_SHOWN);

        if (pWindow)
        { 
            PROFILER_START("sleep", "Sleep Duration");
            SDL_Delay(3000);
            PROFILER_END("sleep");



            SDL_DestroyWindow(pWindow);
        }
        else
        { 
        }
    }

    SDL_Quit();

    return 0;
}