#include "RessourceManager.h"
#include <iostream>

void RessourceManager::PlayMusic(const char* id, int mode)
{
    if (m_musicMap.count(id))
        Mix_PlayMusic(m_musicMap[id], mode);
}

void RessourceManager::PlaySound(const char* id, int mode, int volume)
{
    if (!m_soundMap.count(id))
        return;

    Mix_Chunk* sound = m_soundMap[id];
    Mix_VolumeChunk(sound, volume);
    Mix_PlayChannel(-1, sound, mode);
}

SDL_Texture* RessourceManager::LoadTexture(SDL_Renderer* renderer, const char* path, const char* id)
{
	if (m_textureMap.count(id))
		return m_textureMap[id];

    SDL_Surface* surface = IMG_Load(path);
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

bool RessourceManager::LoadMusic(const char* path, const char* id)
{
    Mix_Music* music = Mix_LoadMUS(path);
    if (!music)
        return false;

    m_musicMap[id] = music;
    return true;
}

bool RessourceManager::LoadSound(const char* path, const char* id)
{
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound)
        return false;

    m_soundMap[id] = sound;
    return true;
}

void RessourceManager::Init(SDL_Renderer* renderer)
{
    InitTextureFolder(renderer);
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

        LoadTexture(renderer,entry.path().string().c_str(), entry.path().stem().string().c_str());  // .string().c_str() ? i don't like that
    }
}


void RessourceManager::DeleteMusic(const char* id)
{
    if (!m_musicMap.contains(id))
        return;

    Mix_FreeMusic(m_musicMap[id]);
    m_musicMap.erase(id);
}

void RessourceManager::DeleteAllMusic()
{
    for (auto& pair : m_musicMap)
    {
        Mix_FreeMusic(pair.second);
    }

    m_musicMap.clear();
}

void RessourceManager::DeleteSound(const char* id)
{
    if (!m_soundMap.contains(id))
        return;

    Mix_FreeChunk(m_soundMap[id]);
    m_soundMap.erase(id);
}

void RessourceManager::DeleteAllSound()
{
    for (auto& pair : m_soundMap)
    {
        Mix_FreeChunk(pair.second);
    }

    m_soundMap.clear();
}

void RessourceManager::DeleteTexture(const char* id)
{
    if (!m_textureMap.count(id))
        return;

    SDL_DestroyTexture(m_textureMap[id]);
    m_textureMap.erase(id);
}

void RessourceManager::DeleteAllTexture()
{
    for (auto& pair : m_textureMap)
    {
        SDL_DestroyTexture(pair.second);
    }
    m_textureMap.clear();
}