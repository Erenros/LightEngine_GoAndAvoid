#pragma once

#include <string>

struct MIX_Audio;
struct MIX_Track;

class Music
{
private:

	MIX_Audio* mp_audio = nullptr;
	MIX_Track* mp_track = nullptr;

public:

	bool IsMusicInit() { return mp_audio == nullptr ? false : true; };

	Music(const std::string& path);
	Music() = default;

	~Music();

	void InitMusic(const std::string& path);

	void PlayMusic(int mode);
};