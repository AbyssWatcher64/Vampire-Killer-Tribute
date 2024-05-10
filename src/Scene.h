#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "EnemyManager.h"
#include "ShotManager.h"
#include "ParticleManager.h"
#include "Text.h"
#include "FadeTransition.h"
#include "Zombie.h"

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

    void CheckObjectCollisions();
    void CheckEnemyCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    
    void RenderGUI() const;

    unsigned int timer;
    Player* player;
    Enemy* enemy;
    int currentLevel;

    //Level structure that contains all the static tiles
    TileMap* level;
    TileMap* levelInteractables;

    //Dynamic objects of the level: items and collectables
    std::vector<Object*> objects;

    //Enemies present in the level
    EnemyManager* enemies;
    bool zombieActive1;
    bool zombieActive2;
    Enemy* zombiesLeft[2];

    //Shots thrown by enemies
    ShotManager* shots;

    ParticleManager* particles;

    Camera2D camera;
    DebugMode debug;

    GameState state;
    
    Text* font;
    FadeTransition fade_transition;
};

