#include "EnemyManager.h"
#include "Zombie.h"
#include "Bat.h"
#include "FishMan.h"
#include "BlackLeopard.h"
#include "Pyre.h"
#include "Candle.h"
#include "DestroyableBlocks.h"
#include "Door.h"

EnemyManager::EnemyManager()
{
	shots = nullptr;
	playerGettingHurt = false;
	particles = nullptr;
	solidEnemy = false;
	playerHasWhiteKey = false;
	damageToDo = 0;
	//totalEnemies = 0;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "img/enemies.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}
	enemyHit = data[19];
	return AppStatus::OK;
}
void EnemyManager::SetParticleManager(ParticleManager* particles)
{
	this->particles = particles;
}
void EnemyManager::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy;

	//TODO: Add our enemies
	if (type == EnemyType::ZOMBIE)
	{
		enemy = new Zombie(pos, ZOMBIE_PHYSICAL_WIDTH, ZOMBIE_PHYSICAL_HEIGHT, ZOMBIE_FRAME_SIZE_WIDTH, ZOMBIE_FRAME_SIZE_HEIGHT);
		enemy->map = this->map;
	}
	else if (type == EnemyType::FISHMAN)
	{
		enemy = new Fishman(pos, FISHMAN_PHYSICAL_WIDTH, FISHMAN_PHYSICAL_HEIGHT, FISHMAN_FRAME_SIZE_WIDTH, FISHMAN_FRAME_SIZE_HEIGHT);
		enemy->map = this->map;
	}
	else if (type == EnemyType::BAT)
	{
		enemy = new Bat(pos, BAT_PHYSICAL_WIDTH, BAT_PHYSICAL_HEIGHT, BAT_FRAME_SIZE_WIDTH, BAT_FRAME_SIZE_HEIGHT);
		enemy->map = this->map;
	}
	else if (type == EnemyType::BLACKLEOPARD)
	{
		enemy = new BlackLeopard(pos, BLACKLEOPARD_PHYSICAL_WIDTH, BLACKLEOPARD_PHYSICAL_HEIGHT, BLACKLEOPARD_FRAME_SIZE_WIDTH, BLACKLEOPARD_FRAME_SIZE_HEIGHT, look);
		enemy->map = this->map;
	}
	else if (type == EnemyType::PYRE)
	{
		enemy = new Pyre(pos, PYRE_PHYSICAL_SIZE, PYRE_PHYSICAL_SIZE, PYRE_PHYSICAL_SIZE, PYRE_PHYSICAL_SIZE);
		enemy->map = this->map;
	}
	else if (type == EnemyType::CANDLE)
	{
		enemy = new Candle(pos, CANDLE_PHYSICAL_SIZE, CANDLE_PHYSICAL_SIZE, CANDLE_PHYSICAL_SIZE, CANDLE_PHYSICAL_SIZE);
		enemy->map = this->map;
	}
	else if (type == EnemyType::BLOCKS)
	{
		enemy = new DestroyableBlock(pos, BLOCKS_FRAME_SIZE, BLOCKS_FRAME_SIZE, BLOCKS_FRAME_SIZE, BLOCKS_FRAME_SIZE);
		enemy->map = this->map;
	}
	else if (type == EnemyType::DOOR)
	{
		enemy = new Door(pos, DOOR_FRAME_SIZE_WIDTH, DOOR_FRAME_SIZE_HEIGHT, DOOR_FRAME_SIZE_WIDTH, DOOR_FRAME_SIZE_HEIGHT);
		enemy->map = this->map;
	}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}
	/*if (type == EnemyType::SLIME)
	{
		enemy = new Slime(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE);
	}
	else if (type == EnemyType::TURRET)
	{
		enemy = new Turret(pos, TURRET_PHYSICAL_WIDTH, TURRET_PHYSICAL_HEIGHT, TURRET_FRAME_SIZE, TURRET_FRAME_SIZE);
	}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}*/

	enemy->Initialise(look, area);
	enemies.push_back(enemy);
}
bool EnemyManager::GetIsSolidEnemy() const
{
	return solidEnemy;
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::SLIME)
	{
		width = SLIME_PHYSICAL_WIDTH;
		height = SLIME_PHYSICAL_HEIGHT;
	}
	else if (type == EnemyType::TURRET)
	{
		width = TURRET_PHYSICAL_WIDTH;
		height = TURRET_PHYSICAL_HEIGHT;
	}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox, const AABB& weapon_hitbox, int& score)
{
	AABB box;
	bool shoot;
	Point p, d;
	//TODO: Add enemy collisions.
	//box = enemy->GetHitbox();

	// TODO: Give tilemap to enemies to give collisions
	//for (int i=0;i<enemies.size();++i)
	//{
	//	shoot = enemies[i]->Update(player_hitbox);
	//	if (shoot)
	//	{
	//		enemies[i]->GetShootingPosDir(&p, &d);
	//		shots->Add(p, d);
	//	}
	//}

	for (Enemy* enemy : enemies)
	{
		if (playerHasWhiteKey)
			enemy->playerHasWhiteKey = true;
		else
			enemy->playerHasWhiteKey = false;
		
		shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
		}

		if (enemy->type == EnemyType::DOOR || enemy->type == EnemyType::BLOCKS)
		{
			solidEnemy = true;
		}
/*	else if (map->TestCollisionWallLeft(box))
		{
			if (look == Look::LEFT)
				look = Look::RIGHT;
		}
		else if (map->TestCollisionWallRight(box))
		{
			if (look == Look::RIGHT)
				look = Look::LEFT;
		}
		else if (map->TestCollisionWallDown(box))
		{

		}*/
	}

	for (Enemy* enemy : enemies)
	{
		box = enemy->GetHitbox();
		solidEnemy = false;
		if (box.TestAABB(player_hitbox) && enemy->IsAlive())
		{
			if (enemy->type != EnemyType::PYRE && enemy->type != EnemyType::CANDLE && enemy->type != EnemyType::BLOCKS && enemy->type != EnemyType::DOOR)
			{
				damageToDo = enemy->damage;
				playerGettingHurt = true;
			}
			if (enemy->type == EnemyType::DOOR)
			{
				if (playerHasWhiteKey)
					enemy->openDoor = true;
			}
		}

		if (box.pos.x + 70 == player_hitbox.pos.x || box.pos.x - 70 == player_hitbox.pos.x)
			enemy->inArea = true;

		if (box.TestAABB(weapon_hitbox))
		{
			if (true && enemy->IsAlive() == true)
			{
				// Added for particle management
				Point p;
				p.x = box.pos.x - (TILE_SIZE - SHOT_PHYSICAL_WIDTH) / 2;
				p.y = box.pos.y - (TILE_SIZE - SHOT_PHYSICAL_HEIGHT) / 2;
				if (enemy->type == EnemyType::ZOMBIE || enemy->type == EnemyType::BAT || enemy->type == EnemyType::BLACKLEOPARD)
				{
					score += 100;
					particles->Add(p);
					totalEnemies--;
				}
				else if (enemy->type == EnemyType::FISHMAN)
				{
					score += 200;
					particles->Add(p);
					totalEnemies--;
				}
				else if (enemy->type == EnemyType::CANDLE || enemy->type == EnemyType::PYRE)
				{
					particles->Add(p);
				}
				if (enemy->type != EnemyType::DOOR)
				{
					PlaySound(enemyHit);
					enemy->SetAlive(false);
				}
			}
		}

		if (box.pos.x == 0)
		{
			enemy->SetAlive(false);
			totalEnemies--;
		}

		if ((box.pos.x < 0 - BLACKLEOPARD_FRAME_SIZE_WIDTH || box.pos.x > WINDOW_WIDTH - BLACKLEOPARD_FRAME_SIZE_WIDTH) && enemy->type == EnemyType::BLACKLEOPARD )
		{
			enemy->SetAlive(false);
			totalEnemies--;
		}
		if (box.pos.x == WINDOW_WIDTH-ZOMBIE_FRAME_SIZE_WIDTH && enemy->type != EnemyType::DOOR)
		{
			enemy->SetAlive(false);
			totalEnemies--;
		}

		if (box.pos.y > 200 && enemy->type == EnemyType::FISHMAN)
		{
			enemy->SetAlive(false);
			totalEnemies--;
		}
		
		// TODO check collisions from enemies with walls and ground, but with type being ZOMBIE / BAT whatever

		// TODO Fix insane memory leak by zombies spawning indefinitely
		//if (enemy->IsAlive() == false)
		//{
		//	if (totalEnemies <= 0)
		//	{
		//		delete enemy;
		//		enemies.clear();
		//	}
		//}
	}
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		if (enemy->IsAlive())
		{
			enemy->Draw();
		}
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		if (enemy->IsAlive() && (enemy->type == EnemyType::PYRE || enemy->type == EnemyType::CANDLE))
		{
			enemy->DrawVisibilityArea(DARKGRAY);
			enemy->DrawHitbox(ORANGE);
		}
		else if (enemy->IsAlive() && (enemy->type == EnemyType::BLOCKS || enemy->type == EnemyType::DOOR))
		{
			enemy->DrawVisibilityArea(DARKGRAY);
			enemy->DrawHitbox(BLUE);
		}
		else if (enemy->IsAlive())
		{
			enemy->DrawVisibilityArea(DARKGRAY);
			enemy->DrawHitbox(RED);
		}
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}