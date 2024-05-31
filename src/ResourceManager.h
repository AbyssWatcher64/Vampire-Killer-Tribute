#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
    IMG_MENU,
    IMG_INTRO,
    IMG_INITIAL,
    IMG_DESC,
    IMG_ENDING,
    IMG_UI,
    IMG_PLAYER, 
    IMG_ENEMIES,
    IMG_TILES,
    IMG_ITEMS,
    IMG_FONT,
    IMG_CLOUD
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }


    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void ReleaseTexture(Resource id);

    void LoadSounds();
    void ReleaseSounds();

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    Sound& operator[](int index)
    {
        return sounds[index];
    }

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    Sound sounds[37];
    
    //sounds[0] = "sfx/24.wav";
    //shieldSFX = LoadSound("sfx/24.wav");
    //attackSFX = LoadSound("sfx/08.wav");
    //moneyBagSFX = LoadSound("sfx/23.wav");
    //orbSFX = LoadSound("music/test1.mp3");

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
};