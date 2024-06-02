#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
}
ResourceManager::~ResourceManager()
{
    Release();
}

//Load a texture from a file and associate it with the key id
AppStatus ResourceManager::LoadTexture(Resource id, const std::string& file_path)
{
    //Load the texture
    Texture2D texture = ::LoadTexture(file_path.c_str());
    if (texture.id == 0)
    {
        //Error loading texture
        LOG("Failed to load texture ", file_path);
        return AppStatus::ERROR;
    }
    
    //Insert the loaded texture into the map with the specified key
    textures[id] = texture;
    return AppStatus::OK;
}

//Release the texture associated with the key id
void ResourceManager::ReleaseTexture(Resource id)
{
    //Find the texture associated with the key
    auto it = textures.find(id);

    //If found, unload the texture and remove it from the map
    if (it != textures.end())
    {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

// Load Sounds:
void ResourceManager::LoadSounds()
{
    sounds[0] = LoadSound("sfx/01.wav");
    sounds[1] = LoadSound("sfx/02.wav");
    sounds[2] = LoadSound("sfx/03.wav");
    sounds[3] = LoadSound("sfx/04.wav");
    sounds[4] = LoadSound("sfx/05.wav");
    sounds[5] = LoadSound("sfx/06.wav");
    sounds[6] = LoadSound("sfx/07.wav");
    sounds[7] = LoadSound("sfx/08.wav");
    sounds[8] = LoadSound("sfx/09.wav");
    sounds[9] = LoadSound("sfx/10.wav");
    sounds[10] = LoadSound("sfx/11.wav");
    sounds[11] = LoadSound("sfx/12.wav");
    sounds[12] = LoadSound("sfx/13.wav");
    sounds[13] = LoadSound("sfx/14.wav");
    sounds[14] = LoadSound("sfx/15.wav");
    sounds[15] = LoadSound("sfx/16.wav");
    sounds[16] = LoadSound("sfx/17.wav");
    sounds[17] = LoadSound("sfx/18.wav");
    sounds[18] = LoadSound("sfx/19.wav");
    sounds[19] = LoadSound("sfx/20.wav");
    sounds[20] = LoadSound("sfx/21.wav");
    sounds[21] = LoadSound("sfx/22.wav");
    sounds[22] = LoadSound("sfx/23.wav");
    sounds[23] = LoadSound("sfx/24.wav");
    sounds[24] = LoadSound("sfx/25.wav");
    sounds[25] = LoadSound("sfx/26.wav");
    sounds[26] = LoadSound("sfx/27.wav");
    sounds[27] = LoadSound("sfx/28.wav");
    sounds[28] = LoadSound("sfx/29.wav");
    sounds[29] = LoadSound("sfx/30.wav");
    sounds[30] = LoadSound("sfx/31.wav");
    sounds[31] = LoadSound("sfx/32.wav");
    sounds[32] = LoadSound("sfx/33.wav");
    sounds[33] = LoadSound("sfx/34.wav");
    sounds[34] = LoadSound("sfx/35.wav");
    sounds[35] = LoadSound("sfx/36.wav");
    sounds[36] = LoadSound("sfx/37.wav");
}
void ResourceManager::ReleaseSounds()
{
    for (int i = 0; i < 36; i++)
    {
        UnloadSound(sounds[i]);
    }
}

void ResourceManager::LoadMusic(int songNumber)
{
    if (songNumber != currentSongIndex)
    {
        if (currentSongIndex != NULL)
            UnloadMusicStream(currentSong);
        StopMusicStream(currentSong);
        switch (songNumber)
        {
        case 1:
            currentSongIndex = 1;
            currentSong = LoadMusicStream("music/01Prologue.ogg");
            currentSong.looping = false;
            break;
        case 2:
            currentSongIndex = 2;
            currentSong = LoadMusicStream("music/02VampireKiller.ogg");
            currentSong.looping = true;
            break;
        case 3:
            currentSongIndex = 3;
            currentSong = LoadMusicStream("music/03PoisonMind.ogg");
            currentSong.looping = true;
            break;
        case 4:
            currentSongIndex = 4;
            currentSong = LoadMusicStream("music/04StageClear.ogg");
            currentSong.looping = false;
            break;
        case 6:
            currentSongIndex = 6;
            currentSong.looping = false;
            currentSong = LoadMusicStream("music/06GameOver.ogg");
            
            break;
        case 7:
            currentSongIndex = 7;
            currentSong = LoadMusicStream("music/07AllClear.ogg");
            currentSong.looping = false;
            break;
        case 8:
            currentSongIndex = 8;
            currentSong = LoadMusicStream("music/08Unused.ogg");
            currentSong.looping = false;
            break;
        case 9:
            currentSongIndex = 9;
            currentSong = LoadMusicStream("music/09PlayerMiss.ogg");
            currentSong.looping = false;
            break;
        }
    }
}

void ResourceManager::StopCurrentSong()
{
    StopMusicStream(currentSong);
}

void ResourceManager::UpdateCurrentSong()
{
    UpdateMusicStream(currentSong);
}
void ResourceManager::PlayCurrentSong()
{
    PlayMusicStream(currentSong);
}
void ResourceManager::ReleaseSong()
{
    PlayMusicStream(currentSong);
}

void ResourceManager::PlaySFX(int sfxInt) const
{
    PlaySound(sounds[sfxInt]);
}

//Get a texture by key
const Texture2D* ResourceManager::GetTexture(Resource id) const
{
    //Find the texture associated with the key and return it
    auto it = textures.find(id);
    if (it != textures.end())   return &(it->second);

    //Return nullptr if key is not found
    return nullptr;
}

void ResourceManager::Release()
{
    for (auto& pair : textures)
    {
        UnloadTexture(pair.second);
    }
    textures.clear();
}