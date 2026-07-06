#pragma once

#include <string>

struct MIX_Audio;
struct MIX_Track;

class Sound
{
private:

	MIX_Audio* mp_audio = nullptr;
	MIX_Track* mp_track = nullptr;

public:

	bool IsSoundInit() { return mp_audio == nullptr ? false : true; }

	Sound(const std::string& path);
	Sound() = default;
	~Sound();

	void InitSound(const std::string& path);

	void PlaySound(int mode, int volume);

};