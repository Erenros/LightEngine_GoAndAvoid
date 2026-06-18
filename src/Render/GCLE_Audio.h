#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

namespace AudioTest
{
	void SetMusicVolumeA(int volume) { Mix_VolumeMusic(volume); };

	void StopMusicA() { Mix_HaltMusic(); };
	void PauseMusicA() { Mix_PauseMusic(); };
	void ResumeMusicA() { Mix_ResumeMusic(); };

	void StopAllSoundA() { Mix_HaltChannel(-1); };

	bool IsMusicPlayingA() { return (Mix_PlayingMusic() ? true : false); };
	bool IsMusicPausedA() { return (Mix_PausedMusic() ? true : false); }
};