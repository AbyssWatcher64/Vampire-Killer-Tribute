#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>


Game::Game()
{
    state = GameState::INITIAL_SCREEN;
    scene = nullptr;
    cutscene = nullptr;
    img_menu = nullptr;
    img_intro = nullptr;
    img_initial = nullptr;
    img_desc = nullptr;
    img_ending = nullptr;
    img_ui = nullptr;
    

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
    if (cutscene != nullptr)
    {
        cutscene->Release();
        delete cutscene;
        cutscene = nullptr;
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

    //Initialise the fade in effect
    //fade_transition.Set(GameState::MAIN_MENU, 5, dst); Por culpa de esto no aparecían las pantallas iniciales

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    cutscene = new Cutscene();
    if (cutscene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (cutscene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }
    else {
        cutscene->Init();
    }

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "img/uielements.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    /*if (data.LoadTexture(Resource::IMG_INTRO, "img/misc.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_intro = data.GetTexture(Resource::IMG_INTRO);*/

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
    if (data.LoadTexture(Resource::IMG_UI, "img/ui.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_ui = data.GetTexture(Resource::IMG_UI);

    if (data.LoadTexture(Resource::IMG_ENDING, "img/ending.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_ending = data.GetTexture(Resource::IMG_ENDING);

    data.LoadSounds();

    
    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    ResourceManager& data = ResourceManager::Instance();
    scene = new Scene();
    

    //PlayMusicStream(Ost2VampireKiller);
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
    ResourceManager& data = ResourceManager::Instance();
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{

    ResourceManager& data = ResourceManager::Instance();
    data.UpdateCurrentSong();

    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;


    if (fade_transition.IsActive())
    {
        GameState prev_frame = state;
        state = fade_transition.Update();

        //Begin play and finish play are delayed due to the fading transition effect
        if (prev_frame == GameState::MAIN_MENU && state == GameState::PLAYING)
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
        }
        else if (prev_frame == GameState::PLAYING && state == GameState::MAIN_MENU)
        {
            FinishPlay();
        }
    }
    else
    {
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
            data.StopCurrentSong();
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                cutscene->Init();
                state = GameState::INTRO;
                //fade_transition.Set(GameState::MAIN_MENU, 5, GameState::PLAYING, 5, dst);
                //PlayMusicStream(Ost2VampireKiller); //No sé si ponerlo en Game o Scene
            }
            break;

        case GameState::INTRO:
            data.LoadMusic(1);
            data.PlayCurrentSong();
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            cutscene->UpdateCutscene();
            if (cutscene->cutsceneFinished==true)
            {
                if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
                fade_transition.Set(GameState::INTRO, 5, GameState::PLAYING, 5, dst);
            }
            break;

        case GameState::PLAYING:
            if (scene->GetIsCurrentlyResetting() == true)
            {
                data.LoadMusic(9);
                data.PlayCurrentSong();
            }
            else if (scene->GetCurrentLevel() < 23)
            {
                data.LoadMusic(2);
                data.PlayCurrentSong();
            }
            else if (scene->GetCurrentLevel() == 23)
            {
                data.LoadMusic(3);
                data.PlayCurrentSong();
            }
           
            if (IsKeyPressed(KEY_ESCAPE))
            {
                //FinishPlay();
                //state = GameState::MAIN_MENU;
                fade_transition.Set(GameState::PLAYING, 5, GameState::MAIN_MENU, 5, dst);

                //StopMusicStream(Ost2VampireKiller);
            }
            else if (IsKeyPressed(KEY_F3))
            {
                state = GameState::GAME_OVER;
                
            }
            else if (scene->GameOver() == true)
            {
                state = GameState::GAME_OVER;
            }
            else if (IsKeyPressed(KEY_F4))
            {
                state = GameState::ENDING;
            }
            else if (scene->GameEnd() == true)
            {
                state = GameState::ENDING;
            }
            else
            {
                scene->Update();
                return AppStatus::OK;

            }
            break;

        case GameState::GAME_OVER:
            data.LoadMusic(6);
            data.PlayCurrentSong();
            if (IsKeyPressed(KEY_ESCAPE))
                return AppStatus::QUIT;
            else if (IsKeyPressed(KEY_SPACE))
                state = GameState::MAIN_MENU;
            break;
        case GameState::ENDING:
            data.LoadMusic(8);
            data.PlayCurrentSong();
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) {
                state = GameState::MAIN_MENU;
            }
            break;
        }
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
            DrawTexturePro(*img_menu, { WINDOW_WIDTH, WINDOW_HEIGHT_OLD_MARGIN /5, WINDOW_WIDTH / 2.3f ,WINDOW_HEIGHT_OLD_MARGIN / 26 }, { 4.875f * (WINDOW_WIDTH / 16),WINDOW_HEIGHT_OLD_MARGIN - (WINDOW_HEIGHT_OLD_MARGIN / 3.6f),WINDOW_WIDTH/2.3f,WINDOW_HEIGHT_OLD_MARGIN / 26 }, { 0,0 }, 0, WHITE);
            break;
        case GameState::INTRO:
            /*DrawTexturePro(*img_intro, { 8,246,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);*/
            cutscene->Render();
            break;
        case GameState::PLAYING:
            scene->Render();
            DrawTexturePro(*img_ui, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            break;
        case GameState::GAME_OVER:
            DrawTexturePro(*img_menu, { 400,89,73,7 }, { WINDOW_WIDTH/2.75f,WINDOW_HEIGHT_OLD_MARGIN /2.25f,73,7 }, { 0,0 }, 0, WHITE);
            break;
        case GameState::ENDING:
            DrawTexturePro(*img_ending, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT_OLD_MARGIN }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT_OLD_MARGIN }, { 0,0 }, 0, WHITE);
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    if (fade_transition.IsActive()) fade_transition.Render();
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
    /*data.ReleaseTexture(Resource::IMG_INTRO);*/
    data.ReleaseTexture(Resource::IMG_INITIAL);
    data.ReleaseTexture(Resource::IMG_DESC);
    data.ReleaseTexture(Resource::IMG_ENDING);
    data.ReleaseTexture(Resource::IMG_UI);
    data.ReleaseSong();
    data.ReleaseSounds();


    UnloadRenderTexture(target);
    //UnloadMusicStream(Ost2VampireKiller);
}