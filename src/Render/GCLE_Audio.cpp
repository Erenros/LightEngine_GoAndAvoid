#include "GCLE_Audio.h"
#include <SDL.h>
#include <SDL_mixer.h>

void Audio::SetMusicVolume(int volume)
{
    Mix_VolumeMusic(volume);
}

void Audio::StopMusic()
{
    Mix_HaltMusic();
}

void Audio::PauseMusic()
{
    Mix_PauseMusic();
}

void Audio::ResumeMusic()
{
    Mix_ResumeMusic();
}

void Audio::StopAllSound()
{
    Mix_HaltChannel(-1);
}

bool Audio::IsAMusicPlaying()
{
    return (Mix_PlayingMusic() ? true : false);
}

bool Audio::IsAMusicPaused()
{
    return (Mix_PausedMusic() ? true : false);
}      