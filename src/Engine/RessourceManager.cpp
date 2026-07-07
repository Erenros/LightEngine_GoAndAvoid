#include "RessourceManager.h"
#include "Texture.h"
#include "Utils.h"
#include "SceneManager.h"
#include "AssetEngine.h"


#undef PlaySound


RessourceManager& RessourceManager::GetInstance() {
	static RessourceManager instance;
	return instance;
}


void RessourceManager::PlayMusic(const std::string& id, int32 mode)
{
	if (m_MusicMap.count(id))
		m_MusicMap[id]->PlayMusic(mode);
}

void RessourceManager::PlaySoundEffect(const std::string& id, int32 mode, int32 volume)
{
	if (!m_SoundMap.count(id))
		return;

	m_SoundMap[id]->PlaySound(mode, volume);
}

void RessourceManager::ForcePutTexture(Sprite* pText, std::string id)
{
	m_TextureMap[id].texture = pText;
}

Font* RessourceManager::GetFont(const std::string& id)
{
	return m_FontMap[id];
}

TextureStruct* RessourceManager::GetTexture(const std::string& id)
{
	if (!m_TextureMap.contains(id))
		return nullptr;

	return &m_TextureMap[id];
}

Sprite* RessourceManager::LoadTexture(Window* pWindow, const std::string& path, const std::string& id)
{
	if (m_TextureMap[id].texture != nullptr)
		return m_TextureMap[id].texture;

	Sprite* texture = new Sprite(pWindow, path);
	if (texture == nullptr || !texture->IsTextureInit())
	{
		GCLE_WARN << "Got a nullptr Texture for path : " + path << ENDL;
		delete texture;
		return nullptr;
	}

	GCLE_INFO << "Texture created" << ENDL;
	m_TextureMap[id].texture = texture;
	AssetEngine::GetInstance().AddAsset(id, texture);
	return texture;
}

bool RessourceManager::LoadMusic(const std::string& path, const std::string& id)
{
	Music* music = new Music(path);
	if (music == nullptr || !music->IsMusicInit())
	{
		GCLE_WARN << "Got nullptr music for path " + path << ENDL;
		delete music;
		return false;
	}

	m_MusicMap[id] = music;
	return true;
}

bool RessourceManager::LoadSound(const std::string& path, const std::string& id)
{
	Sound* sound = new Sound(path);
	if (sound == nullptr || !sound->IsSoundInit())
	{
		GCLE_WARN << "Got a nullptr sound for path : " + path << ENDL;
		delete sound;
		return false;
	}

	m_SoundMap[id] = sound;
	return true;
}

bool RessourceManager::LoadFont(const std::string& path, const std::string& id)
{
	Font* font = new Font(path);
	if (font == NULL)
	{
		GCLE_WARN << "Got a nullptr font for path " + path << ENDL;
		delete font;
		return false;
	}

	m_FontMap[id] = font;
	return true;
}

void RessourceManager::Init(Window* pWindow)
{
	InitTextureFolder(pWindow);
	InitMusicFolder();
	InitSoundFolder();
	InitFont();
}

void RessourceManager::InitTextureFolder(Window* pWindow)
{
	std::filesystem::path filename = "../../assets/textures";

	if (!std::filesystem::exists(filename) || !std::filesystem::is_directory(filename))
	{
		GCLE_WARN << "Directory " + filename.string() + "  does not exist" << ENDL;
		return;
	}

	for (auto& entry : std::filesystem::directory_iterator(filename))
	{
		if (!entry.is_regular_file())
		{
			GCLE_WARN << "File is not valid : " + entry.path().filename().string() << ENDL;
			continue;
		}

		if (entry.path().extension() != ".png")
		{
			GCLE_WARN << "Extension is not correct, expected '.png' receive : " + entry.path().extension().string() << ENDL;
			continue;
		}

        //LoadTexture(window,entry.path().string(), entry.path().stem().string());
        m_TextureMap[entry.path().stem().string()].texture = nullptr;
    }
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

        if (entry.path().extension() != ".png")
        {
            std::cout << "Extension is not correct, expected : .png, receive" + entry.path().extension().string();
            continue;
        }

        m_FontMap[entry.path().stem().string()] = GCLE_NEW Font(filename.string() +"/" + entry.path().filename().string());
    }
}

void RessourceManager::DeleteAll()
{
	DeleteAllFont();
	DeleteAllMusic();
	DeleteAllSound();
	DeleteAllTexture();
}

void RessourceManager::DeleteFont(const std::string& id)
{
	if (!m_FontMap.contains(id))
		return;

	delete m_FontMap[id];
	m_FontMap.erase(id);
}

void RessourceManager::DeleteAllFont()
{
	for (auto& pair : m_FontMap)
		delete pair.second;

	m_FontMap.clear();
}

void RessourceManager::DeleteMusic(const std::string& id)
{
	if (!m_MusicMap.contains(id))
		return;

	delete m_MusicMap[id];
	m_MusicMap.erase(id);
}

void RessourceManager::DeleteAllMusic()
{
	for (auto& pair : m_MusicMap)
		delete pair.second;

	m_MusicMap.clear();
}

void RessourceManager::DeleteSound(const std::string& id)
{
	if (!m_SoundMap.contains(id))
		return;

	delete m_SoundMap[id];
	m_SoundMap.erase(id);
}

void RessourceManager::DeleteAllSound()
{
	for (auto& pair : m_SoundMap)
		delete pair.second;

	m_SoundMap.clear();
}

void RessourceManager::DeleteTexture(const std::string& id)
{
	if (!m_TextureMap.count(id))
		return;

	AssetEngine::GetInstance().DeleteAsset(id);
	delete m_TextureMap[id].texture;
	m_TextureMap[id].texture = nullptr;
	m_TextureMap.erase(id);
}

void RessourceManager::DeleteAllTexture()
{
	for (auto& pair : m_TextureMap)
	{
		if (pair.second.texture)
		{
			delete pair.second.texture;
			pair.second.texture = nullptr;
		}
	}

	m_TextureMap.clear();
}

RessourceManager::~RessourceManager()
{
	DeleteAll();
}

void RessourceManager::SetMusicVolume(int32 volume) { 
	Audio::SetMusicVolume(volume); 
}

void RessourceManager::StopMusic() { 
	Audio::StopMusic(); 
}

void RessourceManager::PauseMusic() { 
	Audio::PauseMusic();
}

void RessourceManager::ResumeMusic() { 
	Audio::ResumeMusic(); 
}

void RessourceManager::StopAllSound() { 
	Audio::StopAllSound(); 
}

bool RessourceManager::isMusicPlaying() {
	return Audio::IsAMusicPlaying();
}

bool RessourceManager::IsMusicPaused() { 
	return Audio::IsAMusicPaused(); 
}