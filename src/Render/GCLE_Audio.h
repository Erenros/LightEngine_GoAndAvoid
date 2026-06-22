#pragma once

class Audio
{
public:
	static void SetMusicVolumeA(int volume)		;
												;
	static void StopMusicA()					;
    static void PauseMusicA()					;
	static void ResumeMusicA()					;
												;
	static void StopAllSoundA()					;
												;
	static bool IsMusicPlayingA()				;
	static bool IsMusicPausedA()				;
};