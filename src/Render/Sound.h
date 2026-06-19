#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class Sound
{
private:

	Mix_Chunk* mp_sound = nullptr;

public:

	bool IsSoundInit() { return mp_sound == nullptr ? false : true; }

	Sound(const std::string& path);
	Sound() = default;
	~Sound() { Mix_FreeChunk(mp_sound); };

	void InitSound(const std::string& path);

	void PlaySound(int mode, int volume);

};