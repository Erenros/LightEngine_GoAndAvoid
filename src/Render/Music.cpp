#include "Music.h"
#include "Utils.h"
#include "GCLE_Audio.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

Music::Music(const std::string& path)
{
	InitMusic(path);
}

Music::~Music()
{
	if (mp_track != nullptr)
		MIX_DestroyTrack(mp_track);
	if (mp_audio != nullptr)
		MIX_DestroyAudio(mp_audio);
}

void Music::InitMusic(const std::string& path)
{
	MIX_Mixer* mixer = Audio::GetMixer();
	if (mixer == nullptr)
	{
		GCLE_WARN << "Mixer not initialized, can't load music with path : " + path << ENDL;
		return;
	}
	 
	MIX_Audio* audio = MIX_LoadAudio(mixer, path.c_str(), false);
	if (audio == NULL)
	{
		GCLE_WARN << "Couldn't inititialize music with path : " + path << ENDL;
		return;
	}

	mp_audio = audio;
	mp_track = MIX_CreateTrack(mixer);
	if (mp_track != nullptr)
		MIX_TagTrack(mp_track, "music");
}

void Music::PlayMusic(int mode)
{
	if (mp_audio == nullptr || mp_track == nullptr)
	{
		GCLE_WARN << "Music is not inititialize can't play it" << ENDL;
		return;
	}

	MIX_SetTrackAudio(mp_track, mp_audio);

	SDL_PropertiesID options = SDL_CreateProperties();
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, mode); 
	MIX_PlayTrack(mp_track, options);
	SDL_DestroyProperties(options);

	Audio::SetCurrentMusicTrack(mp_track);
}