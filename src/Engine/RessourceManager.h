#pragma once
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <string>
#include "Render/Sprite.h"
#include "Render/Sound.h"
#include "Render/Music.h"
#include "Render/Font.h"
#include "Render/GCLE_Audio.h"
#include "include.h"

class Window;
//Faudra biennnn optimiser ca plus tard

struct TextureStruct {
public:
	Sprite* mp_texture = nullptr;
	
private:
	int32 m_useCount = 0;
	bool m_destroyWhenUnused = true;
	uint32 m_flag = 0b0;
		
public:
	void SetFlag(uint32 flag) {
		m_flag = flag;
	}

	void AddFlag(uint32 flag) {
		m_flag |= flag;
	}

	uint32 GetFlag() {
		return m_flag;
	}
	
	void UnloadTexture() {
		if(mp_texture != nullptr)
			delete mp_texture;
			mp_texture = nullptr;
	}

	bool IsLoaded() {
		return mp_texture == nullptr;
	}

	~TextureStruct() {
		delete mp_texture;
	}

};

//Temporaire
//struct Sprite
//{
//	/*bool SpriteSheet = true;
//
//	SDL_Texture* pTexture = nullptr;
//	SDL_Rect SourceRect;
//	
//	float Duration;
//	float Timer;
//
//	int Row, Column;
//	int CurrentRow = 0;
//	int CurrentColumn = 0;
//
//	Sprite(const std::string& id, SDL_Rect sourceRect, bool is_spritesheet = false, int row = 1, int column = 1, float duration = 0.5f);
//
//	void PlayAnimation(int nbr);
//	void UpdateAnimation(float deltaTime = 0.f);*/
//};



/*
Dans l'id�e �a donnera un truc du style 
	EntityPtr* e = CreateEntity.... ect
		
	e->AddSprite(GCLE_NEW Sprite( TextureID / Texture* , bool spritesheet, int row, int col, srect, duration ) 
*/




class RessourceManager
{
private:

	std::unordered_map<std::string, Sound*> m_soundMap;
	std::unordered_map<std::string, Music*> m_musicMap;

	std::unordered_map<std::string, TextureStruct> m_textureMap;

	std::unordered_map<std::string, Font*> m_fontMap;

	friend class SceneManager;

public:

	static RessourceManager& GetInstance() {
		static RessourceManager instance;
		return instance;
	}

	Font* GetFont(const std::string& id);
	TextureStruct* GetTexture(const std::string& id) { return (m_textureMap.count(id) ? &m_textureMap[id] : nullptr); };
	Sprite* LoadTexture(Window* window, const std::string& path, const std::string& id);

	void SetMusicVolume(int32 volume) { Audio::SetMusicVolume(volume); };

	void StopMusic() { Audio::StopMusic(); };
	void PauseMusic() { Audio::PauseMusic(); };
	void ResumeMusic() { Audio::ResumeMusic(); };

	void StopAllSound() { Audio::StopAllSound(); };

	bool isMusicPlaying() { return Audio::IsAMusicPlaying(); };
	bool IsMusicPaused() { return Audio::IsAMusicPaused(); }

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

	bool LoadFont(const std::string& path, const std::string& id);

	void Init(Window* window);


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