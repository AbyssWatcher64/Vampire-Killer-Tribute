#include "Weapon.h"
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include <raymath.h>

Weapon::Weapon(const Point& p, WeaponType t) : Entity(p, LOGICAL_WHIP_WIDTH_SIZE, LOGICAL_WHIP_HEIGHT_SIZE, LOGICAL_WHIP_WIDTH_SIZE, LOGICAL_WHIP_HEIGHT_SIZE)
{
	type = t;
	width = LOGICAL_WHIP_WIDTH_SIZE;
	height = LOGICAL_WHIP_HEIGHT_SIZE;
	this->pos.x = p.x;
	this->pos.y = p.x;
}

Weapon::~Weapon()
{
}

AppStatus Weapon::Initialise()
{
	return AppStatus::OK;
}

void Weapon::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}