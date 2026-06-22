#include "GCLE_Audio.h"
#include <SDL.h>
#include <SDL_mixer.h>

void Audio::SetMusicVolumeA(int volume)
{
    Mix_VolumeMusic(volume);
}

void Audio::StopMusicA()
{
    Mix_HaltMusic();
}

void Audio::PauseMusicA()
{
    Mix_PauseMusic();
}

void Audio::ResumeMusicA()
{
    Mix_ResumeMusic();
}

void Audio::StopAllSoundA()
{
    Mix_HaltChannel(-1);
}

bool Audio::IsMusicPlayingA()
{
    return (Mix_PlayingMusic() ? true : false);
}

bool Audio::IsMusicPausedA()
{
    return (Mix_PausedMusic() ? true : false);
}      