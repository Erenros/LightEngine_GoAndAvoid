#include "AudioListener.h"

void AudioListener::SetMusicVolume(int32 volume)
{
	RessourceManager::GetInstance().SetMusicVolume(volume);
}

void AudioListener::StopMusic()
{
	RessourceManager::GetInstance().StopMusic();
}

void AudioListener::PauseMusic()
{
	RessourceManager::GetInstance().PauseMusic();
}

void AudioListener::ResumeMusic()
{
	RessourceManager::GetInstance().ResumeMusic();
}

void AudioListener::StopAllSound()
{
	RessourceManager::GetInstance().StopAllSound();
}

bool AudioListener::IsMusicPlaying()
{
	return RessourceManager::GetInstance().isMusicPlaying();
}

bool AudioListener::IsMusicPaused()
{
	return RessourceManager::GetInstance().IsMusicPaused();
}

void AudioListener::PlayMusic(const std::string& id, int32 mode)
{
	RessourceManager::GetInstance().PlayMusic(id, mode);
}

void AudioListener::PlaySoundEffect(const std::string& id, int32 mode, int32 volume)
{
	RessourceManager::GetInstance().PlaySoundEffect(id, mode, volume);
}
