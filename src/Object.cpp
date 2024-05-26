#include "Object.h"
#include "Sprite.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
	//case ObjectType::APPLE: rc = { 4 * n, 3 * n, n, n }; break;
	//case ObjectType::CHILI: rc = { 5 * n, 3 * n, n, n }; break;
	case ObjectType::MORNINGSTAR: rc = { 0 * n, 0 * n, n, n }; break;
	case ObjectType::AXE: rc = { 1 * n, 0 * n, 2 * n, n }; break;
	case ObjectType::DAGGER: rc = { 3 * n, 0 * n, n, n }; break;
	case ObjectType::BLUECROSS: rc = { 4 * n, 0 * n, n, n }; break;
	case ObjectType::HOURGLASS: rc = { 5 * n, 0 * n, n, n }; break;
	case ObjectType::HOLYWATER: rc = { 6 * n, 0 * n, n, n }; break;
	case ObjectType::MASTERKEY: rc = { 7 * n, 0 * n, n, n }; break;
	case ObjectType::ORB: rc = { 8 * n, 0 * n, n, n }; break;
	case ObjectType::CHEST: rc = { 0 * n, 1 * n, n, n }; break;
	case ObjectType::YELLOWKEY: rc = { 1 * n, 1 * n, n, n }; break;
	case ObjectType::WHITEKEY: rc = { 2 * n, 1 * n, n, n }; break;
	case ObjectType::HEART: rc = { 3 * n, 1 * n, n, n }; break;
	case ObjectType::BIGHEART: rc = { 3 * n, 2 * n, n, n }; break;
	case ObjectType::WHITEBAG: rc = { 8 * n, 1 * n, n, n }; break;
	case ObjectType::BLUEBAG: rc = { 9 * n, 1 * n, n, n }; break;
	

	case ObjectType::SHIELD: rc = { 2 * n, 3 * n, n, n }; break;
	//case ObjectType::FIRE: rc = { 0 * n, 4 * n, n, n }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_ITEMS), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
// returns Points depending on the item grabbed
int Object::Points() const
{
	//if (type == ObjectType::APPLE)		return POINTS_APPLE;
	//else if (type == ObjectType::CHILI)	return POINTS_CHILI;
	if (type == ObjectType::WHITEBAG) 
	{
		return POINTS_WHITEBAG;
	}
	else if (type == ObjectType::BLUEBAG) 
	{
		return POINTS_BLUEBAG;
	}
	else
	{

		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}

int Object::Equip() const
{
	if (type == ObjectType::SHIELD)		return EQUIPMENT_NUMBER_SHIELD;
	if (type == ObjectType::ORB) 		return EQUIPMENT_WINNING_ORB;
}

int Object::ObjectNum() const
{
	if (type == ObjectType::WHITEBAG || type == ObjectType::BLUEBAG)
		return 1;
	else if (type == ObjectType::ORB)
		return 2;
	else if (type == ObjectType::HEART)
		return 3;
	else if (type == ObjectType::BIGHEART)
		return 4;
}






