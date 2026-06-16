#include "RessourceManager.h"
#include <iostream>

void RessourceManager::PlayMusic(const std::string& id, int mode)
{
    if (m_musicMap.count(id))
        Mix_PlayMusic(m_musicMap[id], mode);
}

void RessourceManager::PlaySound(const std::string& id, int mode, int volume)
{
    if (!m_soundMap.count(id))
        return;

    Mix_Chunk* sound = m_soundMap[id];
    Mix_VolumeChunk(sound, volume);
    Mix_PlayChannel(-1, sound, mode);
}

SDL_Texture* RessourceManager::LoadTexture(SDL_Renderer* renderer, const std::string& path, const std::string& id)
{
	if (m_textureMap.count(id))
		return m_textureMap[id];

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL)
    {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        return nullptr;
    }

    SDL_FreeSurface(surface);

    std::cout << "Texture created" << std::endl;
    m_textureMap[id] = texture;
    return texture;
}

void RessourceManager::SetFontSize(const std::string& id, int size)
{
    if (!m_fontMap.contains(id))
        return;

    TTF_SetFontSize(m_fontMap[id], size);
}

bool RessourceManager::LoadMusic(const std::string& path, const std::string& id)
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music)
        return false;

    m_musicMap[id] = music;
    return true;
}

bool RessourceManager::LoadSound(const std::string& path, const std::string& id)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound)
        return false;

    m_soundMap[id] = sound;
    return true;
}

bool RessourceManager::LoadFont(const std::string& path, const std::string& id, int size)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (!font)
        return false;

    m_fontMap[id] = font;
    return false;
}

void RessourceManager::Init(SDL_Renderer* renderer)
{
    InitTextureFolder(renderer);
    InitMusicFolder();
    InitSoundFolder();
    InitFont();
}

void RessourceManager::InitTextureFolder(SDL_Renderer* renderer)
{
    std::filesystem::path filename = "../../assets/textures";

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

        LoadTexture(renderer,entry.path().string(), entry.path().stem().string());
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

        if (entry.path().extension() != ".ttf")
        {
            std::cout << "Extension is not correct, expected : .wav, receive" + entry.path().extension().string();
            continue;
        }

        LoadFont(entry.path().string(), entry.path().stem().string(), 25);
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
    if (!m_fontMap.contains(id))
        return;

    TTF_CloseFont(m_fontMap[id]);
    m_fontMap.erase(id);
}

void RessourceManager::DeleteAllFont()
{
    for (auto& pair : m_fontMap)
        TTF_CloseFont(pair.second);

    m_fontMap.clear();
}

void RessourceManager::DeleteMusic(const std::string& id)
{
    if (!m_musicMap.contains(id))
        return;

    Mix_FreeMusic(m_musicMap[id]);
    m_musicMap.erase(id);
}

void RessourceManager::DeleteAllMusic()
{
    for (auto& pair : m_musicMap)
        Mix_FreeMusic(pair.second);

    m_musicMap.clear();
}

void RessourceManager::DeleteSound(const std::string& id)
{
    if (!m_soundMap.contains(id))
        return;

    Mix_FreeChunk(m_soundMap[id]);
    m_soundMap.erase(id);
}

void RessourceManager::DeleteAllSound()
{
    for (auto& pair : m_soundMap)
        Mix_FreeChunk(pair.second);

    m_soundMap.clear();
}

void RessourceManager::DeleteTexture(const std::string& id)
{
    if (!m_textureMap.count(id))
        return;

    SDL_DestroyTexture(m_textureMap[id]);
    m_textureMap.erase(id);
}

void RessourceManager::DeleteAllTexture()
{
    for (auto& pair : m_textureMap)
        SDL_DestroyTexture(pair.second);

    m_textureMap.clear();
}

void SpriteSheet::PlayAnimation(int nbr)
{
    if (nbr >= row) nbr = row;
    if (nbr < row) nbr = 0;

    currentRow = nbr;
    srect.y = currentRow * srect.h;
}

void SpriteSheet::UpdateAnimation(float deltaTime = 0.f)
{
    currentColumn += 1;
    if (currentColumn >= column) currentColumn = 0;

    srect.x = currentColumn * srect.w;
}
