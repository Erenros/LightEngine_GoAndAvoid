#pragma once

struct MIX_Mixer;
struct MIX_Track;

class Audio
{
public: 
	static bool Init(); 
	static void Shutdown();

	static MIX_Mixer* GetMixer() { return s_mixer; } 
	static void SetCurrentMusicTrack(MIX_Track* track) { s_musicTrack = track; }

	static void SetMusicVolume(int volume);
	static void StopMusic();
	static void PauseMusic();
	static void ResumeMusic();
	static void StopAllSound();
	static bool IsAMusicPlaying();
	static bool IsAMusicPaused();

private:
	static MIX_Mixer* s_mixer;
	static MIX_Track* s_musicTrack;
};