#include "Sound.h"
#include "Utils.h"
#include <SDL.h>
#include <SDL_mixer.h>


Sound::Sound(const std::string& path)
{
	InitSound(path);
}

Sound::~Sound(){
    Mix_FreeChunk(mp_sound);
}

void Sound::InitSound(const std::string& path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == NULL)
    {
        GCLE_WARN << "Couldn't initialize sound with path : " + path << ENDL;
        return;
    }
    
    mp_sound = sound;
}

void Sound::PlaySound(int mode, int volume)
{
    if (mp_sound == nullptr)
    {
        GCLE_WARN << "Sound doesn't have a value" << ENDL;
        return;
    }

    Mix_VolumeChunk(mp_sound, volume);
    Mix_PlayChannel(-1, mp_sound, mode);
}
