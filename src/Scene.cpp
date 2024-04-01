#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		//player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	
	if(stage == 1)
	{
		size = LEVEL_WIDTH * LEVEL_HEIGHT;
		int map[] = {
				 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,  13,
				 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   9,
				10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,
				 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   9,
				10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,  10,
				 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   9,
				10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17,  18,  17,  18,  22,  23,	  0,   0,   0,  10,
				 9,  11,  12,   0,   0,  13,   0,   0,  13,  17,  18,  17,  18,   0,   0,   0,   0,   0,   0,  20,  21,	  0,   0,   0,   9,
				10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,	  0,   0,   0,  10,
				 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,	  0,   0,   0,   9,
				10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  22,  23,   9,   0,  20,  21,	  0,   0,   0,  10,
				 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,  10,   0,  20,  21,	  0,   0,   0,   9,
				10,   0,  -1,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,   9,   0,  20,  21,	  0,   0,   0,  10,
				 9,   0, 100,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,  10,   0,  20,  21,	  0,   0,   0,   9,
				10,  17,  18,  17,  18,  13,   0,   0,   0,   0,  13,  17,  18,  13,   0,  20,  21,   9,   0,  20,  21,	  0,   0,   0,  10,
				 9,   1,   2,   5,   6,  40,  70,  70,  70,  70,  41,   1,   2,   9,   0,  20,  21,  10,   0,  20,  21,	  0,   0,   0,   9,
				10,   3,   4,   7,   8,  13,  14,  15,  16,  11,  12,   3,   4,  10,  14,  15,  16,  13,  14,  15,  16,   0,  13,   0,  10
		};
		//Entities
		i = 0;
		for (y = 0; y < LEVEL_HEIGHT; ++y)
		{
			for (x = 0; x < LEVEL_WIDTH; ++x)
			{
				tile = (Tile)map[i];
				if (tile == Tile::PLAYER)
				{
					pos.x = x * TILE_SIZE;
					pos.y = y * TILE_SIZE - (PLAYER_FRAME_SIZE - TILE_SIZE);
					player->SetPos(pos);
				}
				++i;
			}
		}
		//Tile map
		level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}
	return AppStatus::OK;
}
void Scene::HandleInputPlayer()
{
	bool moving = false;

	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}

	switch(player->GetState())
	{
		case State::IDLE:
			if (IsKeyDown(KEY_LEFT))		player->StartWalkingLeft();
			else if (IsKeyDown(KEY_RIGHT))	player->StartWalkingRight();
			break;

		case State::WALKING:
			if (player->IsLookingRight())
			{
				if (IsKeyDown(KEY_RIGHT))
				{
					//continue walking right
					moving = true;
				}
				else if (IsKeyDown(KEY_LEFT))
				{
					player->StartWalkingLeft();
					moving = true;
				}
			}
			else
			{
				if (IsKeyDown(KEY_LEFT))
				{
					//continue walking left
					moving = true;
				}
				else if (IsKeyDown(KEY_RIGHT))
				{
					player->StartWalkingRight();
					moving = true;
				}
			}
			if(!moving)	player->Stop();
			break;
	}
}
void Scene::Update()
{
	level->Update();
	player->Update();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();

	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
		player->Draw();
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
		player->DrawDebug(GREEN);

	EndMode2D();
}
void Scene::Release()
{
    level->Release();
	player->Release();
}