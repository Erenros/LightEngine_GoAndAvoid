#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <string>

//Faudra biennnn optimiser ca plus tard

class RessourceManager
{
private:

	std::unordered_map<std::string, Mix_Chunk*> m_soundMap;
	std::unordered_map<std::string, Mix_Music*> m_musicMap;

	std::unordered_map<std::string, SDL_Texture*> m_textureMap;

	std::unordered_map<std::string, TTF_Font*> m_fontMap;

public:

	static RessourceManager& GetInstance() {
		static RessourceManager instance;
		return instance;
	}

	SDL_Texture* GetTexture(const std::string& id) { return (m_textureMap.count(id) ? m_textureMap[id] : nullptr); };
	SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path, const std::string& id);

	TTF_Font* GetFont(const std::string& id) { return m_fontMap.contains(id) ? m_fontMap[id] : nullptr; }

	void SetFontSize(const std::string& id, int size);

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
	void PlayMusic(const std::string& id, int mode);
	/// <summary>
	/// Mode : 1 = 1 fois,  0 = 1 boucle, -1 = infinite loop
	/// </summary>
	/// <param name="id"></param>
	/// <param name="mode"></param>
	void PlaySound(const std::string& id, int mode, int volume);

	bool LoadMusic(const std::string& path, const std::string& id);
	bool LoadSound(const std::string& path, const std::string& id);

	bool LoadFont(const std::string& path, const std::string& id, int size);

	void Init(SDL_Renderer* renderer);


	//I'm just gonna make only one function for this four later
	void InitTextureFolder(SDL_Renderer* renderer);
	void InitMusicFolder();
	void InitSoundFolder();
	void InitFont();

	void DeleteAll();
	void DeleteFont(const std::string& id);
	void DeleteAllFont();
	void DeleteMusic(const std::string& id);
	void DeleteAllMusic();
	void DeleteSound(const std::string& id);
	void DeleteAllSound();
	void DeleteTexture(const std::string& id);
	void DeleteAllTexture();

	~RessourceManager() { DeleteAll(); };
};