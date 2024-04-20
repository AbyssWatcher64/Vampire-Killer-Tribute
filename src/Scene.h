#pragma once
#include <raylib.h>
#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Object.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

    void ResetScreen();
    bool GameOver();
    bool GameEnd();

private:
    AppStatus LoadLevel(int stage);

    int currentLevel;

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    Enemy* enemy;
    TileMap *level;
    std::vector<Object*> objects;
    
    std::vector<Enemy*> enemies;
    //Enemy** enemies;

    Camera2D camera;
    DebugMode debug;
};

