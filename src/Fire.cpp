#include "Sprite.h"
#include "Fire.h"

Fire::Fire(const Point& p, FireType t) : Entity(p, FIRE_PHYSICAL_SIZE, FIRE_PHYSICAL_SIZE, FIRE_FRAME_SIZE, FIRE_FRAME_SIZE)
{
	type = t;
	Rectangle rc;
	const int n = TILE_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	//render = new StaticImage(data.GetTexture(Resource::IMG_ITEMS), rc);
}
Fire::~Fire()
{
}
void Fire::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}