#include "GCLE_Audio.h"
#include "Utils.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

MIX_Mixer* Audio::s_mixer = nullptr;
MIX_Track* Audio::s_musicTrack = nullptr;

bool Audio::Init()
{
	if (!MIX_Init())
	{
		GCLE_WARN << "MIX_Init failed : " << SDL_GetError() << ENDL;
		return false;
	}

	s_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (s_mixer == nullptr)
	{
		GCLE_WARN << "MIX_CreateMixerDevice failed : " << SDL_GetError() << ENDL;
		return false;
	}

	return true;
}

void Audio::Shutdown()
{
	MIX_Quit(); 
	s_mixer = nullptr;
	s_musicTrack = nullptr;
}

void Audio::SetMusicVolume(int volume)
{
	if (s_musicTrack != nullptr)
		MIX_SetTrackGain(s_musicTrack, static_cast<float>(volume) / 128.0f);
}

void Audio::StopMusic()
{
	if (s_musicTrack != nullptr)
		MIX_StopTrack(s_musicTrack, 0);
}

void Audio::PauseMusic()
{
	if (s_musicTrack != nullptr)
		MIX_PauseTrack(s_musicTrack);
}

void Audio::ResumeMusic()
{
	if (s_musicTrack != nullptr)
		MIX_ResumeTrack(s_musicTrack);
}

void Audio::StopAllSound()
{
	if (s_mixer != nullptr)
		MIX_StopTag(s_mixer, "sfx", 0);
}

bool Audio::IsAMusicPlaying()
{
	return s_musicTrack != nullptr && MIX_TrackPlaying(s_musicTrack);
}

bool Audio::IsAMusicPaused()
{
	return s_musicTrack != nullptr && MIX_TrackPaused(s_musicTrack);
}