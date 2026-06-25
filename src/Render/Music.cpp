#include "Music.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_mixer.h>

Music::Music(const std::string& path)
{
	InitMusic(path);
}

Music::~Music()
{
    Mix_FreeMusic(mp_music);
}

void Music::InitMusic(const std::string& path)
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music == NULL)
    {
        GCLE_WARN << "Couldn't inititialize music with path : " + path << ENDL;
        return;
    }

    mp_music = music;
}



void Music::PlayMusic(int mode)
{
    if (mp_music == nullptr)
    {
        GCLE_WARN << "Music is not inititialize can't play it" << ENDL;
        return;
    }

    Mix_PlayMusic(mp_music, mode);
}
