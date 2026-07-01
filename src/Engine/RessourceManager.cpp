#include "RessourceManager.h"
#include "Texture.h"
#include "Utils.h"
#include "SceneManager.h"
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

Sprite* RessourceManager::LoadTexture(Window* window, const std::string& path, const std::string& id)
{
	if (m_textureMap[id].mp_texture != nullptr)
		return m_textureMap[id].mp_texture;

    Sprite* texture = GCLE_NEW Sprite(window, path);
    if (texture == nullptr || !texture->IsTextureInit())
    {
        GCLE_WARN << "Got a nullptr Texture for path : " + path << ENDL;
        delete texture;
        return nullptr;
    }
     
    GCLE_INFO << "Texture created" << ENDL;
    m_textureMap[id].mp_texture = texture;
    return texture;
}

void RessourceManager::SetFontSize(const std::string& id, int size)
{
    if (!m_fontMap.contains(id))
        return;

    m_fontMap[id]->SetFontSize(size);
}

bool RessourceManager::LoadMusic(const std::string& path, const std::string& id)
{
    Music* music = GCLE_NEW Music(path);
    if (music == nullptr || !music->IsMusicInit())
    {
        GCLE_WARN << "Got nullptr music for path " + path << ENDL;
        delete music;
        return false;
    }

    m_musicMap[id] = music;
    return true;
}

bool RessourceManager::LoadSound(const std::string& path, const std::string& id)
{
    Sound* sound = GCLE_NEW Sound(path);
    if (sound == nullptr || !sound->IsSoundInit())
    {
        GCLE_WARN << "Got a nullptr sound for path : " + path << ENDL;
        delete sound;
        return false;
    }

    m_soundMap[id] = sound;
    return true;
}

bool RessourceManager::LoadFont(const std::string& path, const std::string& id)
{
    Font* font = GCLE_NEW Font(path);
    if (font == NULL)
    {
        GCLE_WARN << "Got a nullptr font for path " + path << ENDL;
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

        //LoadTexture(window,entry.path().string(), entry.path().stem().string());
        m_textureMap[entry.path().stem().string()].mp_texture = nullptr;
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

        LoadFont(entry.path().string(), entry.path().stem().string());
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

void RessourceManager::DeleteTexture(const std::string& id)
{
    if (!m_textureMap.count(id))
        return;

    delete m_textureMap[id].mp_texture;
    m_textureMap[id].mp_texture = nullptr;
    m_textureMap.erase(id);
}

void RessourceManager::DeleteAllTexture()
{
    for (auto& pair : m_textureMap)
    {
        if (pair.second.mp_texture)
        {
            delete pair.second.mp_texture;
            pair.second.mp_texture = nullptr;
        }
    }

    m_textureMap.clear();
}

//Sprite::Sprite(const std::string& id, SDL_Rect sourceRect, bool is_spritesheet, int row, int column, float duration) :
//    SpriteSheet(is_spritesheet),
//    Row(row),
//    Column(column),
//    SourceRect(sourceRect),
//    Duration(duration),
//    Timer(duration)
//{
//    pTexture = RessourceManager::GetInstance().GetTexture(id)->GetSDLTexture();
//}
//
//void Sprite::PlayAnimation(int nbr)
//{
//    if (!SpriteSheet) return;
//
//    if (nbr >= Row) nbr = Row;
//    if (nbr < Row) nbr = 0;
//
//    CurrentRow = nbr;
//    SourceRect.y = CurrentRow * SourceRect.h;
//}
//
//void Sprite::UpdateAnimation(float deltaTime)
//{
//    if (!SpriteSheet) return;
//
//    Timer -= deltaTime;
//
//    //-Waiting for class Deltatime-
//    //if (Timer > 0)
//    //    return;
//
//    //Timer = Duration;
//
//    CurrentColumn += 1;
//    if (CurrentColumn >= Column) CurrentColumn = 0;
//
//    SourceRect.x = CurrentColumn * SourceRect.w;
//}
