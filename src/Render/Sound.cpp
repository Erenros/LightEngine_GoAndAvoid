#include "Sound.h"
#include "Utils.h"
#include "GCLE_Audio.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>


Sound::Sound(const std::string& path)
{
	InitSound(path);
}

Sound::~Sound()
{
	if (mp_track != nullptr)
		MIX_DestroyTrack(mp_track);
	if (mp_audio != nullptr)
		MIX_DestroyAudio(mp_audio);
}

void Sound::InitSound(const std::string& path)
{
	MIX_Mixer* mixer = Audio::GetMixer();
	if (mixer == nullptr)
	{
		GCLE_WARN << "Mixer not initialized, can't load sound with path : " + path << ENDL;
		return;
	}
	 
	MIX_Audio* audio = MIX_LoadAudio(mixer, path.c_str(), true);
	if (audio == NULL)
	{
		GCLE_WARN << "Couldn't initialize sound with path : " + path << ENDL;
		return;
	}

	mp_audio = audio;
	mp_track = MIX_CreateTrack(mixer);
	if (mp_track != nullptr)
		MIX_TagTrack(mp_track, "sfx");
}

void Sound::PlaySound(int mode, int volume)
{
	if (mp_audio == nullptr || mp_track == nullptr)
	{
		GCLE_WARN << "Sound doesn't have a value" << ENDL;
		return;
	}

	MIX_SetTrackGain(mp_track, static_cast<float>(volume) / 128.0f);
	MIX_SetTrackAudio(mp_track, mp_audio);

	SDL_PropertiesID options = SDL_CreateProperties();
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, mode);
	MIX_PlayTrack(mp_track, options);
	SDL_DestroyProperties(options);
}