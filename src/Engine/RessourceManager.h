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
	Sprite* texture = nullptr;

private:
	uint32 m_Flag = 0b0;

public:
	void SetFlag(uint32 flag) {
		m_Flag = flag;
	}

	void AddFlag(uint32 flag) {
		m_Flag |= flag;
	}

	uint32 GetFlag() {
		return m_Flag;
	}

	void UnloadTexture() {
		if (texture != nullptr)
			delete texture;
		texture = nullptr;
	}

	bool IsLoaded() {
		return texture == nullptr;
	}

	~TextureStruct() {
		delete texture;
	}

};


class RessourceManager
{
private:

	std::unordered_map<std::string, Sound*> m_SoundMap;
	std::unordered_map<std::string, Music*> m_MusicMap;

	std::unordered_map<std::string, TextureStruct> m_TextureMap;

	std::unordered_map<std::string, Font*> m_FontMap;

	friend class SceneManager;

private:

	void ForcePutTexture(Sprite* pText, std::string id);
	friend class AssetEngine;

public:

	static RessourceManager& GetInstance();

	Font* GetFont(const std::string& id) ;
	TextureStruct* GetTexture(const std::string& id);
	Sprite* LoadTexture(Window* pWindow, const std::string& path, const std::string& id);

	void SetMusicVolume(int32 volume) ;

	void StopMusic()	 ;
	void PauseMusic()	 ;
	void ResumeMusic()	 ;
	void StopAllSound()  ;

	bool isMusicPlaying();
	bool IsMusicPaused() ;

	/// <summary>
	/// Mode : 1 = 1 fois,  0 = 1 boucle, -1 = infinite loop
	/// </summary>
	/// <param name="id"></param>
	/// <param name="mode"></param>
	void PlayMusic(const std::string& id, int32 mode);
	/// <summary>
	/// Mode : 1 = 1 fois,  0 = 1 boucle, -1 = infinite loop
	/// </summary>
	/// <param name="id"></param>
	/// <param name="mode"></param>
	void PlaySoundEffect(const std::string& id, int32 mode, int32 volume);

	bool LoadMusic(const std::string& path, const std::string& id);
	bool LoadSound(const std::string& path, const std::string& id);

	bool LoadFont(const std::string& path, const std::string& id);

	void Init(Window* pWindow);


	void InitTextureFolder(Window* pWindow);
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

	~RessourceManager();
};