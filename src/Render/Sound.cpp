#include "Sound.h"
#include "Utils.h"

Sound::Sound(const std::string& path)
{
	InitSound(path);
}

void Sound::InitSound(const std::string& path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == NULL)
    {
        DEBUG_WARN << "Couldn't initialize sound with path : " + path << ENDL;
        return;
    }
    
    mp_sound = sound;
}

void Sound::PlaySound(int mode, int volume)
{
    if (mp_sound == nullptr)
    {
        DEBUG_WARN << "Sound doesn't have a value" << ENDL;
        return;
    }

    Mix_VolumeChunk(mp_sound, volume);
    Mix_PlayChannel(-1, mp_sound, mode);
}
