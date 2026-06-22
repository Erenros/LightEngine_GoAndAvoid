#pragma once

#include <string>

struct Mix_Music;

class Music
{
private:

	Mix_Music* mp_music = nullptr;

public:

	bool IsMusicInit() { return mp_music == nullptr ? false : true; };

	Music(const std::string& path);
	Music() = default;

	~Music();

	void InitMusic(const std::string& path);

	void PlayMusic(int mode);
};