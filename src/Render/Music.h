#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class Music
{
private:

	Mix_Music* mp_music = nullptr;

public:

	bool IsMusicInit() { return mp_music == nullptr ? false : true; };

	Music(const std::string& path);
	Music() = default;

	~Music() { Mix_FreeMusic(mp_music); };

	void InitMusic(const std::string& path);

	void PlayMusic(int mode);
};