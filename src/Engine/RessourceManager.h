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
#include "Render/Surface.h"
#include "include.h"

class Window;
//Faudra biennnn optimiser ca plus tard

struct SurfaceStruct {
public:
	Surface* mp_surface = nullptr;
	
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
		if(mp_surface != nullptr)
			delete mp_surface;
			mp_surface = nullptr;
	}

	bool IsLoaded() {
		return mp_surface == nullptr;
	}

	~SurfaceStruct() {
		delete mp_surface;
	}

};


class RessourceManager
{
private:

	std::unordered_map<std::string, Sound*> m_soundMap;
	std::unordered_map<std::string, Music*> m_musicMap;

	std::unordered_map<std::string, SurfaceStruct> m_surfaceMap;

	std::unordered_map<std::string, Font*> m_fontMap;

	std::unordered_map<std::string, std::vector<Texture*>> m_textures;

	friend class SceneManager;
	
private: 

	void ForcePutSurface(Surface* text, std::string id);
	friend class AssetEngine;

public:

	static RessourceManager& GetInstance() {
		static RessourceManager instance;
		return instance;
	}

	Font* GetFont(const std::string& id) { return m_fontMap[id]; };
	SurfaceStruct* GetSurface(const std::string& id);
	Surface* LoadSurface(Window* window, const std::string& path, const std::string& id);

	void AddTexture(const std::string& id, Texture* tex);

	//TTF_Font* GetFont(const std::string& id) { return m_fontMap.contains(id) ? m_fontMap[id] : nullptr; }

	void SetFontSize(const std::string& id, int size);

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

	bool LoadFont(const std::string& path, const std::string& id, int size);

	void Init(Window* window);


	void InitTextureFolder(Window* window);
	void InitMusicFolder();
	void InitSoundFolder();
	void InitFont();

	void EraseTexture(const std::string& id);

	void DeleteAll();
	void DeleteFont(const std::string& id);
	void DeleteAllFont();
	void DeleteMusic(const std::string& id);
	void DeleteAllMusic();
	void DeleteSound(const std::string& id);
	void DeleteAllSound();
	void DeleteSurface(const std::string& id);
	void DeleteAllSurface();

	~RessourceManager() { DeleteAll(); };
};