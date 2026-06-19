#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

class Audio
{
public:
	static void SetMusicVolumeA(int volume) { Mix_VolumeMusic(volume); };

	static void StopMusicA() { Mix_HaltMusic(); };
    static void PauseMusicA() { Mix_PauseMusic(); };
	static void ResumeMusicA() { Mix_ResumeMusic(); };

	static void StopAllSoundA() { Mix_HaltChannel(-1); };

	static bool IsMusicPlayingA() { return (Mix_PlayingMusic() ? true : false); };
	static bool IsMusicPausedA() { return (Mix_PausedMusic() ? true : false); }
};