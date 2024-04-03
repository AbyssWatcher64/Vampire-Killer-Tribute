#pragma once
#include <raylib.h>
#include "Player.h"
#include "Zombie.h"
#include "TileMap.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void HandleInputPlayer();
    void Update();
    void Render();
    void Release();

private:
    AppStatus LoadLevel(int stage);

    Player *player;
    Zombie *zombie;
    TileMap *level;
    Camera2D camera;

    DebugMode debug;
};

