#include "RessourceManager.h"
#include "Texture.h"
#include "Utils.h"
#include "SceneManager.h"
#include "AssetEngine.h"

#undef PlaySound


void RessourceManager::PlayMusic(const std::string& id, int mode)
{
	if (m_musicMap.count(id))
		m_musicMap[id]->PlayMusic(mode);
}

void RessourceManager::PlaySoundEffect(const std::string& id, int mode, int volume)
{
	if (!m_soundMap.count(id))
		return;

	m_soundMap[id]->PlaySound(mode, volume);
}

void RessourceManager::ForcePutSurface(Surface* text, std::string id)
{
	m_surfaceMap[id].mp_surface = text;
}

SurfaceStruct* RessourceManager::GetSurface(const std::string& id)
{
	if (!m_surfaceMap.contains(id))
		return nullptr;

	return &m_surfaceMap[id];
}

Surface* RessourceManager::LoadSurface(Window* window, const std::string& path, const std::string& id)
{
	if (m_surfaceMap[id].mp_surface != nullptr)
		return m_surfaceMap[id].mp_surface;

	Surface* texture = new Surface(window, path);
	if (texture == nullptr || !texture->IsSurfaceInit())
	{
		DEBUG_WARN << "Got a nullptr Surface for path : " + path << ENDL;
		delete texture;
		return nullptr;
	}
	 
	DEBUG_INFO << "Surface created" << ENDL;
	m_surfaceMap[id].mp_surface = texture;
	AssetEngine::GetInstance().AddAsset(id);
	return texture;
}

void RessourceManager::AddTexture(const std::string& id, Texture* tex)
{
	m_textures[id].push_back(tex);
}

void RessourceManager::SetFontSize(const std::string& id, int size)
{
	if (!m_fontMap.contains(id))
		return;

	m_fontMap[id]->SetFontSize(size);
}

bool RessourceManager::LoadMusic(const std::string& path, const std::string& id)
{
	Music* music = new Music(path);
	if (music == nullptr || !music->IsMusicInit())
	{
		DEBUG_WARN << "Got nullptr music for path " + path << ENDL;
		delete music;
		return false;
	}

	m_musicMap[id] = music;
	return true;
}

bool RessourceManager::LoadSound(const std::string& path, const std::string& id)
{
	Sound* sound = new Sound(path);
	if (sound == nullptr || !sound->IsSoundInit())
	{
		DEBUG_WARN << "Got a nullptr sound for path : " + path << ENDL;
		delete sound;
		return false;
	}

	m_soundMap[id] = sound;
	return true;
}

bool RessourceManager::LoadFont(const std::string& path, const std::string& id, int size)
{
	Font* font = new Font(path, size);
	if (font == NULL)
	{
		DEBUG_WARN << "Got a nullptr font for path " + path << ENDL;
		delete font;
		return false;
	}
	
	m_fontMap[id] = font;
	return true;
}

void RessourceManager::Init(Window* window)
{
	InitTextureFolder(window);
	InitMusicFolder();
	InitSoundFolder();
	InitFont();
}

void RessourceManager::InitTextureFolder(Window* window)
{
	std::filesystem::path filename = "../../assets/textures";

	if (!std::filesystem::exists(filename) || !std::filesystem::is_directory(filename))
	{
		DEBUG_WARN << "Directory " + filename.string() + " does not exits" << ENDL;
		return;
	}

	for (auto& entry : std::filesystem::directory_iterator(filename))
	{
		if (!entry.is_regular_file())
		{
			DEBUG_WARN << "File is not valid : " + entry.path().filename().string() << ENDL;
			continue;
		}

		if (entry.path().extension() != ".png")
		{
			DEBUG_WARN << "Extension is not correct, expected '.png' receive : " + entry.path().extension().string() << ENDL;
			continue;
		}

		if (m_surfaceMap.contains(entry.path().stem().string()))
		{
			DEBUG_INFO << "Texture '" << entry.path().stem().string() << "' already loaded" << ENDL;
			continue;
		}

		//LoadTexture(window,entry.path().string(), entry.path().stem().string());
		m_surfaceMap[entry.path().stem().string()].mp_surface = nullptr;
	}

	//for (auto& tex : m_textureMap) {
	//    if (tex.second.GetFlag() & SceneManager::GetInstance().GetCurrentSceneFlag() == 0b0) {
	//        tex.second.UnloadTexture();
	//    }
	//}
}

void RessourceManager::InitMusicFolder()
{
	std::filesystem::path filename = "../../assets/musics";

	if (!std::filesystem::exists(filename) || !std::filesystem::is_directory(filename))
	{
		std::cout << "Directory " + filename.string() + "does not exits" << std::endl;
		return;
	}

	for (auto& entry : std::filesystem::directory_iterator(filename))
	{
		if (!entry.is_regular_file())
		{
			std::cout << "File is not valid : " + entry.path().filename().string();
			continue;
		}

		if (entry.path().extension() != ".mp3")
		{
			std::cout << "Extension is not correct, expected : .mp3, receive" + entry.path().extension().string();
			continue;
		}

		LoadMusic(entry.path().string(), entry.path().stem().string());
	}
}

void RessourceManager::InitSoundFolder()
{
	std::filesystem::path filename = "../../assets/sounds";

	if (!std::filesystem::exists(filename) || !std::filesystem::is_directory(filename))
	{
		std::cout << "Directory " + filename.string() + "does not exits" << std::endl;
		return;
	}

	for (auto& entry : std::filesystem::directory_iterator(filename))
	{
		if (!entry.is_regular_file())
		{
			std::cout << "File is not valid : " + entry.path().filename().string();
			continue;
		}

		if (entry.path().extension() != ".wav")
		{
			std::cout << "Extension is not correct, expected : .wav, receive" + entry.path().extension().string();
			continue;
		}

		LoadSound(entry.path().string(), entry.path().stem().string());
	}
}

void RessourceManager::InitFont()
{
	std::filesystem::path filename = "../../assets/fonts";

	if (!std::filesystem::exists(filename) || !std::filesystem::is_directory(filename))
	{
		std::cout << "Directory " + filename.string() + "does not exits" << std::endl;
		return;
	}

	for (auto& entry : std::filesystem::directory_iterator(filename))
	{
		if (!entry.is_regular_file())
		{
			std::cout << "File is not valid : " + entry.path().filename().string();
			continue;
		}

		if (entry.path().extension() != ".ttf" && entry.path().extension() != ".otf")
		{
			std::cout << "Extension is not correct, expected : .ttf, receive" + entry.path().extension().string();
			continue;
		}

		LoadFont(entry.path().string(), entry.path().stem().string(), 25);
	}
}

void RessourceManager::EraseTexture(const std::string& id)
{
	for (auto& tex : m_textures[id])
		delete tex;

	m_textures[id].clear();
}

void RessourceManager::DeleteAll()
{
	DeleteAllFont();
	DeleteAllMusic();
	DeleteAllSound();
	DeleteAllSurface();
}

void RessourceManager::DeleteFont(const std::string& id)
{
	if (!m_fontMap.contains(id))
		return;

	delete m_fontMap[id];
	m_fontMap.erase(id);
}

void RessourceManager::DeleteAllFont()
{
	for (auto& pair : m_fontMap)
		delete pair.second;

	m_fontMap.clear();
}

void RessourceManager::DeleteMusic(const std::string& id)
{
	if (!m_musicMap.contains(id))
		return;

	delete m_musicMap[id];
	m_musicMap.erase(id);
}

void RessourceManager::DeleteAllMusic()
{
	for (auto& pair : m_musicMap)
		delete pair.second;

	m_musicMap.clear();
}

void RessourceManager::DeleteSound(const std::string& id)
{
	if (!m_soundMap.contains(id))
		return;

	delete m_soundMap[id];
	m_soundMap.erase(id);
}

void RessourceManager::DeleteAllSound()
{
	for (auto& pair : m_soundMap)
		delete pair.second;

	m_soundMap.clear();
}

void RessourceManager::DeleteSurface(const std::string& id)
{
	if (!m_surfaceMap.count(id))
		return;

	AssetEngine::GetInstance().DeleteAsset(id);
	delete m_surfaceMap[id].mp_surface;
	m_surfaceMap[id].mp_surface = nullptr;
	m_surfaceMap.erase(id);
	EraseTexture(id);
}

void RessourceManager::DeleteAllSurface()
{
	for (auto& pair : m_surfaceMap)
	{
		if (pair.second.mp_surface)
		{
			delete pair.second.mp_surface;
			pair.second.mp_surface = nullptr;
			EraseTexture(pair.first);
		}
	}

	m_surfaceMap.clear();
}