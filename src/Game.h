#pragma once
#include "Globals.h"
#include "Scene.h"
#include "FadeTransition.h"

//enum class GameState { INITIAL_SCREEN, DESCRIPTION_SCREEN, MAIN_MENU, PLAYING, SETTINGS, CREDITS, GAME_OVER, ENDING };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* img_intro;
    const Texture2D *img_initial;
    const Texture2D *img_desc;
    const Texture2D *img_ending;
    const Texture2D* img_ui;

    Music Ost2VampireKiller;
    Sound Ost2VampireKillerSound;
    

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;

    //To make fade out/in transitions effects
    FadeTransition fade_transition;
};