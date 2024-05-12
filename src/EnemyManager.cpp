#include "EnemyManager.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "TileMap.h"


EnemyManager::EnemyManager()
{
	shots = nullptr;
	playerGettingHurt = false;
	totalEnemies = 0;
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
	return AppStatus::OK;
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
void EnemyManager::Update(const AABB& player_hitbox, const AABB& weapon_hitbox)
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
		shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
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
		if (box.TestAABB(player_hitbox) && enemy->IsAlive())
		{
			if (true)
				playerGettingHurt = true;
		}

		if (box.TestAABB(weapon_hitbox))
		{
			if (true)
			{
				enemy->SetAlive(false);
			}
		}
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
		if (enemy->IsAlive())
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