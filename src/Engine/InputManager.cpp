#include "InputManager.h"

void InputManager::HandleInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                exit(0);
            }

            case SDL_KEYDOWN:
            {
                if (keyMap[event.key.keysym.sym].isPressed == false)
                {
                    keyMap[event.key.keysym.sym] = { true, false, false };
                }

                else
                {
                    keyMap[event.key.keysym.sym] = { true, true, false };
                }

                break;
            }

            case SDL_KEYUP:
            {
                keyMap[event.key.keysym.sym] = { false, false, true };
                break;
            }
        }
    }
}
