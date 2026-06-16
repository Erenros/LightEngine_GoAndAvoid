#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <filesystem>
#include <fstream>

//Faudra biennnn optimiser ca plus tard

class RessourceManager
{
private:

	std::unordered_map<const char*, Mix_Chunk*> m_soundMap;
	std::unordered_map<const char*, Mix_Music*> m_musicMap;

	std::unordered_map<const char*, SDL_Texture*> m_textureMap;

public:

	static RessourceManager& GetInstance() {
		static RessourceManager instance;
		return instance;
	}

	SDL_Texture* GetTexture(const char* id) { return (m_textureMap.count(id) ? m_textureMap[id] : nullptr); };
	SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path, const char* id);


	void SetMusicVolume(int volume) { Mix_VolumeMusic(volume); };

	void StopMusic() { Mix_HaltMusic(); };
	void PauseMusic() { Mix_PauseMusic(); };
	void ResumeMusic() { Mix_ResumeMusic(); };

	void StopAllSound() { Mix_HaltChannel(-1); };

	bool isMusicPlaying() { return (Mix_PlayingMusic() ? true : false); };
	bool IsMusicPaused() { return (Mix_PausedMusic() ? true : false); }

	/// <summary>
	/// Mode : 1 = 1 fois,  0 = 1 boucle, -1 = infinite loop
	/// </summary>
	/// <param name="id"></param>
	/// <param name="mode"></param>
	void PlayMusic(const char* id, int mode);
	/// <summary>
	/// Mode : 1 = 1 fois,  0 = 1 boucle, -1 = infinite loop
	/// </summary>
	/// <param name="id"></param>
	/// <param name="mode"></param>
	void PlaySound(const char* id, int mode, int volume);

	bool LoadMusic(const char* path, const char* id);
	bool LoadSound(const char* path, const char* id);


	void Init(SDL_Renderer* renderer);

	void InitTextureFolder(SDL_Renderer* renderer);
	void InitMusicFolder();
	void InitSoundFolder();



	void DeleteMusic(const char* id);
	void DeleteAllMusic();
	void DeleteSound(const char* id);
	void DeleteAllSound();
	void DeleteTexture(const char* id);
	void DeleteAllTexture();
};