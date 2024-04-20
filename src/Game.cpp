#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>


Game::Game()
{
    state = GameState::INITIAL_SCREEN;
    scene = nullptr;
    img_menu = nullptr;
    img_initial = nullptr;
    img_desc = nullptr;
    img_ending = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Vampire Killer Tribute");

    //Initialise audio system
    InitAudioDevice();

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);


    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "img/ui.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_INITIAL, "img/initial.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_initial = data.GetTexture(Resource::IMG_INITIAL);

    if (data.LoadTexture(Resource::IMG_DESC, "img/description.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_desc = data.GetTexture(Resource::IMG_DESC);

    if (data.LoadTexture(Resource::IMG_ENDING, "img/ending.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_ending = data.GetTexture(Resource::IMG_ENDING);

    Ost2VampireKiller = LoadMusicStream("music/02VampireKiller.ogg");
    PlayMusicStream(Ost2VampireKiller);
    /*SetMusicVolume(Ost2VampireKiller, 1.0);*/
    
    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
        case GameState::INITIAL_SCREEN:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) {
                state = GameState::DESCRIPTION_SCREEN;
            }
            break;
        case GameState::DESCRIPTION_SCREEN:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) {
                state = GameState::MAIN_MENU;
            }
            break;
        case GameState::MAIN_MENU: 
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
                //PlayMusicStream(Ost2VampireKiller); //No sé si ponerlo en Game o Scene
            }
            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;
                //StopMusicStream(Ost2VampireKiller);
            }
            else if(IsKeyPressed(KEY_F3))
            {
                state = GameState::GAME_OVER;
                //StopMusicStream(Ost2VampireKiller);
            }
            else if (scene->GameOver() == true)
            {
                state = GameState::GAME_OVER;
                //StopMusicStream(Ost2VampireKiller);
            }
            else if (IsKeyPressed(KEY_F4))
            {
                state = GameState::ENDING;
                //StopMusicStream(Ost2VampireKiller);
            }
            else if (scene->GameEnd() == true)
            {
                state = GameState::ENDING;
                //StopMusicStream(Ost2VampireKiller);
            }
            else
            {
                //Game logic
                scene->Update();
                UpdateMusicStream(Ost2VampireKiller);
            }
            break;

        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ESCAPE)) 
                return AppStatus::QUIT;
            else if (IsKeyPressed(KEY_SPACE))
                state = GameState::MAIN_MENU;
            break;
        case GameState::ENDING:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) {
                state = GameState::MAIN_MENU;
            }
            break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::INITIAL_SCREEN:
            DrawTexturePro(*img_initial, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            break;
        case GameState::DESCRIPTION_SCREEN:
            DrawTexturePro(*img_desc, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            break;
        case GameState::MAIN_MENU:
            //DrawTexture(*img_menu, 0, 0, WHITE);
            // This draws the Vampire killer logo in the middle.
            // TODO: Change the numbers of the second curly braces to have divisions so that if you make the screen smaller, it will get smaller as well (not hard-coded)
           
            //// This draws the Push Space Key text
            //DrawTexturePro(*img_menu, { 258, 41, 110,7 }, { WINDOW_WIDTH/3,(WINDOW_HEIGHT - (WINDOW_HEIGHT / 3))+20,110,7 }, { 0,0 }, 0, WHITE);

            DrawTexturePro(*img_menu, { 0, WINDOW_HEIGHT/26, WINDOW_WIDTH,WINDOW_HEIGHT/2 }, { WINDOW_WIDTH / 42 ,WINDOW_WIDTH / 16,WINDOW_WIDTH - WINDOW_WIDTH / 20, WINDOW_HEIGHT / 2.2f }, { 0,0 }, 0, WHITE);

                // This draws the Konami text
            // TODO: Check HEIGHT VS WIDTH ?? Are we using height for width?
            DrawTexturePro(*img_menu, { WINDOW_WIDTH, WINDOW_HEIGHT/13, WINDOW_WIDTH/2.45f,WINDOW_HEIGHT / 26 }, { 4.875f * (WINDOW_WIDTH / 16),WINDOW_HEIGHT - (WINDOW_HEIGHT / 2.5f),WINDOW_WIDTH / 2.45f,WINDOW_HEIGHT / 26 }, { 0,0 }, 0, WHITE);

                // This draws the Push Space Key text
            DrawTexturePro(*img_menu, { WINDOW_WIDTH, WINDOW_HEIGHT/5, WINDOW_WIDTH / 2.3f ,WINDOW_HEIGHT / 26 }, { 4.875f * (WINDOW_WIDTH / 16),WINDOW_HEIGHT - (WINDOW_HEIGHT / 3.6f),WINDOW_WIDTH/2.3f,WINDOW_HEIGHT / 26 }, { 0,0 }, 0, WHITE);
            break;

        case GameState::PLAYING:
            scene->Render();
            break;
        case GameState::GAME_OVER:
            DrawTexturePro(*img_menu, { 400,89,73,7 }, { WINDOW_WIDTH/2.75f,WINDOW_HEIGHT/2.25f,73,7 }, { 0,0 }, 0, WHITE);
            break;
        case GameState::ENDING:
            DrawTexturePro(*img_ending, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    CloseAudioDevice();
    UnloadResources();
    CloseWindow();
}         
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);

    UnloadRenderTexture(target);
    UnloadMusicStream(Ost2VampireKiller);
}