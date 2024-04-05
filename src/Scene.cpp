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
	// Trying zombie
	//int* map;

	//map = new int[4] {, , , , };
	if(stage == 1)
	{
		//TODO: Fill stage with the adequate tiles
		size = LEVEL_WIDTH * LEVEL_HEIGHT;
		int map[] = {
				33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,  
				29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,
				25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,
				21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,
				17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,
				13,		14,		10,		9,		12,		14,		15,		16,		13,		14,		15,		16,		12,		14,		15,		16,
				5,		6,		67,		68,		8,		7,		5,		7,		5,		7,		5,		82,		83,		7,		5,		7,
				3,		100,	65,		66,		800,	81,		4,		81,		4,		81,		4,		81,		800,	81,		3,		81,
				2,		2,		62,		63,		64,		2,		2,		2,		2,		2,		2,		2,		64,		2,		2,		2,
				1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
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
				//TOASK:
				//trying zombie out - It's not working

				/*else if (tile == Tile::ZOMBIE)
				{
					pos.x = x * TILE_SIZE;
					pos.y = y * TILE_SIZE - (ZOMBIE_FRAME_SIZE - TILE_SIZE);
					zombie->SetPos(pos);
				}*/
				++i;
			}
		}
		//Tile map
		level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	}
	else if(stage == 2)
	{
		size = LEVEL_WIDTH * LEVEL_HEIGHT;
		int map[] = {
				33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,
				29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,
				25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,
				21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,
				17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,
				13,		14,		10,		9,		12,		14,		15,		16,		13,		14,		15,		16,		12,		14,		15,		16,
				5,		6,		67,		68,		8,		7,		5,		7,		5,		7,		5,		82,		83,		7,		5,		7,
				3,		100,	65,		66,		800,	81,		4,		81,		4,		81,		4,		81,		800,	81,		3,		81,
				2,		2,		62,		63,		64,		2,		2,		2,		2,		2,		2,		2,		64,		2,		2,		2,
				1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
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
				//trying zombie out - It's not working

				/*else if (tile == Tile::ZOMBIE)
				{
					pos.x = x * TILE_SIZE;
					pos.y = y * TILE_SIZE - (ZOMBIE_FRAME_SIZE - TILE_SIZE);
					zombie->SetPos(pos);
				}*/
				++i;
			}
		}
		//Tile map
		level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	}
	else if (stage == 3) 
	{

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
			if (IsKeyPressed(KEY_SPACE))	player->Attack();
			else if (IsKeyDown(KEY_LEFT))		player->StartWalkingLeft();
			else if (IsKeyDown(KEY_RIGHT))	player->StartWalkingRight();
			else if (IsKeyPressed(KEY_L))	player->Death();
			break;

		case State::WALKING:
			if (player->IsLookingRight())
			{
				if (IsKeyPressed(KEY_SPACE))
					player->Attack();
				else if (IsKeyDown(KEY_RIGHT))
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
				if (IsKeyPressed(KEY_SPACE))
					player->Attack();
				else if (IsKeyDown(KEY_LEFT))
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
		
		case State::ATTACKING:
			moving = false;
			if (player->IsLookingRight())
			{

			}
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