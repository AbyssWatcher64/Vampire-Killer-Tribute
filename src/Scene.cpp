#include "Scene.h"
#include <stdio.h>
#include "Globals.h"


Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	currentLevel = 1;

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
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
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
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Tile tileInteractable;
	Point pos;
	int* map = nullptr;
	int* mapInteractables = nullptr;
	Object* obj;

	ClearLevel();
	
	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if(stage == 1)
	{
		map = new int[size] 
			{
			
				33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,  
				29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,
				25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,
				21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,
				17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,
				13,		14,		10,		9,		12,		14,		15,		16,		13,		14,		15,		16,		12,		14,		15,		16,
				5,		6,		67,		68,		8,		7,		5,		7,		5,		7,		5,		139,	140,	7,		5,		7,
				3,		138,	65,		66,		138,	138,	4,		138,	4,		138,	4,		138,	138/*800*/,		138,	3,		138,
				2,		2,		62,		63,		64,		2,		2,		2,		 2,		2,		2,		2,		64,		2,		2,		2,
				1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
			};
		mapInteractables = new int[size]
			{
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		800,	138,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		200,	0,		0,		0,		0,		0,		300,	0,		400,	401,	0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		800,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
			};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size]
			{
				33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		35,		36,
				29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		31,		32,
				25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		27,		28,
				21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		23,		24,
				17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		19,		20,
				13,		14,		10,		9,		12,		14,		15,		16,		13,		14,		15,		16,		12,		14,		15,		16,
				5,		6,		67,		68,		8,		7,		5,		7,		5,		7,		5,		139,	140,	7,		5,		7,
				3,		138,	65,		66,		138,	138,	4,		138,	4,		138,	4,		138,	138,	138,	3,		138,
				2,		2,		62,		63,		64,		2,		2,		2,		2,		2,		2,		2,		64,		2,		2,		2,
				1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
			};
		mapInteractables = new int[size]
			{
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		800,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		200,	0,		0,		0,		0,		0,		400,	0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
			};
	}
	else if (stage == 3)
	{
		map = new int[size]
			{
				33,		34,		35,		36,		33,		34,		35,		36,		33,		34,		37,		38,		39,		38,		39,		38,
				29,		30,		31,		32,		29,		30,		31,		32,		29,		30,		40,		41,		42,		42,		43,		44,
				25,		26,		27,		28,		25,		26,		27,		28,		25,		26,		45,		46,		47,		48,		43,		49,
				21,		22,		23,		24,		21,		22,		23,		24,		21,		22,		40,		50,		51,		113,	52,		141,
				17,		18,		19,		20,		17,		18,		19,		20,		17,		18,		45,		53,		54,		113,	55,		56,
				13,		14,		10,		9,		12,		14,		15,		16,		13,		14,		40,		50,		51,		113,	52,		141,
				5,		6,		67,		68,		8,		7,		5,		7,		5,		7,		45,		53,		54,		113,	55,		56,
				3,		138,	65,		66,		138,	138,	4,		138,	4,		138,	57,		58,		59,		113,	60,		61,
				2,		2,		62,		63,		64,		2,		2,		2,		2,		2,		57,		142,	59,		113,	60,		143,
				1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,		1,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
		};
		mapInteractables = new int[size]
			{
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		800, 	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		200,	0,		0,		0,		0,		401,	0,		0,		0,		500,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0
			};
	}
	else if (stage == 4)
	{
		map = new int[size]
			{
				94,		95,		102,	103,	72,		72,		78,		79,		72,		82,		86,		85,		72,		72,		74,		75,
				96,		97,		104,	105,	93,		93,		93,		93,		72,		72,		72,		86,		85,		72,		72,		72,
				96,		97,		106,	107,	93,		93,		93,		93,		72,		72,		72,		82,		86,		85,		72,		72,
				96,		97,		108,	72,		93,		93,		93,		93,		72,		72,		72,		72,		72,		86,		85,		72,
				96,		97,		109,	72,		93,		93,		93,		93,		72,		72,		83,		70,		69,		70,		69,		70,
				96,		97,		110,	72,		76,		77,		72,		72,		72,		83,		84,		72,		94,		95,		80,		81,
				96,		97,		110,	72,		74,		75,		83,		70,		69,		70,		73,		72,		96,		97,		78,		79,
				96,		97,		111,	72,		72,		83,		84,		72,		72,		72,		72,		72,		96,		97,		76,		77,
				98,		97,		112,	72,		83,		84,		73,		72,		72,		72,		72,		72,		96,		97,		74,		75,
				100,	101,	72,		83,		84,		72,		72,		72,		72,		72,		72,		72,		96,		97,		72,		72,
				69,		70,		69,		70,		69,		70,		69,		70,		69,		70,		69,		70,		69,		70,		69,		70,
			};
		mapInteractables = new int[size]
			{
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		200,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
				0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
			};	
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			/*else if (tile == Tile::ITEM_APPLE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::APPLE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CHILI)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHILI);
				objects.push_back(obj);
				map[i] = 0;
			}*/
			else if (tile == Tile::ITEM_SHIELD)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::SHIELD);
				objects.push_back(obj);
				map[i] = 0;
			}


			tileInteractable = (Tile)mapInteractables[i];
			if (tileInteractable == Tile::EMPTY)
			{
				mapInteractables[i] = 0;
			}
			else if (tileInteractable == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				mapInteractables[i] = 0;
			}
			//else if (tileInteractable == Tile::ITEM_APPLE)
			//{
			//	pos.x = x * TILE_SIZE;
			//	pos.y = y * TILE_SIZE + TILE_SIZE - 1;
			//	obj = new Object(pos, ObjectType::APPLE);
			//	objects.push_back(obj);
			//	mapInteractables[i] = 0;
			//}
			//else if (tileInteractable == Tile::ITEM_CHILI)
			//{
			//	pos.x = x * TILE_SIZE;
			//	pos.y = y * TILE_SIZE + TILE_SIZE - 1;
			//	obj = new Object(pos, ObjectType::CHILI);
			//	objects.push_back(obj);
			//	mapInteractables[i] = 0;
			//}
			else if (tileInteractable == Tile::ITEM_SHIELD)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::SHIELD);
				objects.push_back(obj);
				mapInteractables[i] = 0;
			}
			else if (tileInteractable == Tile::ITEM_WHITEBAG)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::WHITEBAG);
				objects.push_back(obj);
				mapInteractables[i] = 0;
			}
			else if (tileInteractable == Tile::ITEM_BLUEBAG)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::BLUEBAG);
				objects.push_back(obj);
				mapInteractables[i] = 0;
			}
			else if (tileInteractable == Tile::ITEM_ORB)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ORB);
				objects.push_back(obj);
				mapInteractables[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	//level->Load(mapInteractables, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] mapInteractables;
	delete[] map;

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	else if (IsKeyPressed(KEY_F2))
	{
		player->GodMode();
	}
	else if(IsKeyPressed(KEY_KP_1))
	{
		currentLevel = 1;
		LoadLevel(1);
	}
	else if (IsKeyPressed(KEY_KP_2))
	{
		currentLevel = 2;
		LoadLevel(2);

	}
	else if (IsKeyPressed(KEY_KP_3))
	{
		currentLevel = 3;
		LoadLevel(3);

	}
	else if (IsKeyPressed(KEY_KP_4))
	{
		LoadLevel(4);
	}
	

	if (player->GetXPos() == 0 && currentLevel > 1)
	{
		
		LoadLevel(currentLevel-1);
		player->SetPos(Point(WINDOW_WIDTH - 40, 150));
		currentLevel--;
	}
	else if (player->GetXPos() == WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH && currentLevel < 3)
	{
		
		LoadLevel(currentLevel + 1);
		player->SetPos(Point(20, 150));
		currentLevel++;
	}

	ResetScreen();

	level->Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

	level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
	level->Release();
	player->Release();
	ClearLevel();
}
void Scene::ResetScreen()
{	
	if (player->GetHasDied() == true)
	{
		WaitTime(2);
		LoadLevel(1);
		player->SetHasDied(false);
		player->ChangeHP(100);
	}
}
bool Scene::GameOver()
{
	if (player->GetGameOver() == true)
	{
		player->SetGameOver(false);
		return true;
	}
}
bool Scene::GameEnd()
{
	if (player->GetGameEnd() == true)
	{
		player->SetGameEnd(false);
		return true;
	}
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;

	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			//player->SetShield((*it)->Points());
			player->SetEquipment((*it)->Equip());
			if ((*it)->Equip() == 1000)
				player->SetGameEnd(true);
			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
	DrawText(TextFormat("HP : %d", player->GetHP()), 10, 20, 8, LIGHTGRAY);
	DrawText(TextFormat("LIVES : %d", player->GetLives()), 10, 30, 8, LIGHTGRAY);
}