#pragma once

#include <string>

class Mix_Chunk;

class Sound
{
private:

	Mix_Chunk* mp_sound = nullptr;

public:

	bool IsSoundInit() { return mp_sound == nullptr ? false : true; }

	Sound(const std::string& path);
	Sound() = default;
	~Sound();

	void InitSound(const std::string& path);

	void PlaySound(int mode, int volume);

};