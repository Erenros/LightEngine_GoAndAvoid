#pragma once



class Audio
{
public:
	static void SetMusicVolume(int volume); 
	static void StopMusic();
    static void PauseMusic();
	static void ResumeMusic(); 
	static void StopAllSound(); 
	static bool IsAMusicPlaying();
	static bool IsAMusicPaused();
};