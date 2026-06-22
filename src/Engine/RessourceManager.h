#pragma once
//#include <SDL.h>
//#include <SDL_image.h>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <string>
#include "Render/Texture.h"
#include "Render/Sound.h"
#include "Render/Music.h"
#include "Render/Font.h"
#include "Render/GCLE_Audio.h"

class Window;
//Faudra biennnn optimiser ca plus tard


//Temporaire
struct Sprite
{
	/*bool SpriteSheet = true;

	SDL_Texture* pTexture = nullptr;
	SDL_Rect SourceRect;
	
	float Duration;
	float Timer;

	int Row, Column;
	int CurrentRow = 0;
	int CurrentColumn = 0;

	Sprite(const std::string& id, SDL_Rect sourceRect, bool is_spritesheet = false, int row = 1, int column = 1, float duration = 0.5f);

	void PlayAnimation(int nbr);
	void UpdateAnimation(float deltaTime = 0.f);*/
};



/*
Dans l'idée ça donnera un truc du style 
	Entity* e = CreateEntity.... ect
		
	e->AddSprite(new Sprite( TextureID / Texture* , bool spritesheet, int row, int col, srect, duration ) 
*/




class RessourceManager
{
private:

	std::unordered_map<std::string, Sound*> m_soundMap;
	std::unordered_map<std::string, Music*> m_musicMap;

	std::unordered_map<std::string, Texture*> m_textureMap;

	std::unordered_map<std::string, Font*> m_fontMap;

public:

	static RessourceManager& GetInstance() {
		static RessourceManager instance;
		return instance;
	}

	Font* GetFont(const std::string& id) { return m_fontMap[id]; };
	Texture* GetTexture(const std::string& id) { return (m_textureMap.count(id) ? m_textureMap[id] : nullptr); };
	Texture* LoadTexture(Window* window, const std::string& path, const std::string& id);

	//TTF_Font* GetFont(const std::string& id) { return m_fontMap.contains(id) ? m_fontMap[id] : nullptr; }

	void SetFontSize(const std::string& id, int size);

	void SetMusicVolume(int volume) { Audio::SetMusicVolumeA(volume); };

	void StopMusic() { Audio::StopMusicA(); };
	void PauseMusic() { Audio::PauseMusicA(); };
	void ResumeMusic() { Audio::ResumeMusicA(); };

	void StopAllSound() { Audio::StopAllSoundA(); };

	bool isMusicPlaying() { return Audio::IsMusicPlayingA(); };
	bool IsMusicPaused() { return Audio::IsMusicPausedA(); }

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
	void PlaySoundEffect(const std::string& id, int mode, int volume);

	bool LoadMusic(const std::string& path, const std::string& id);
	bool LoadSound(const std::string& path, const std::string& id);

	bool LoadFont(const std::string& path, const std::string& id, int size);

	void Init(Window* window);


	//I'm just gonna make only one function for this four later
	void InitTextureFolder(Window* window);
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