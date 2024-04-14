#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

#define POINTS_APPLE	10
#define POINTS_CHILI	20
#define POINTS_BLUEBAG 1000
#define POINTS_WHITEBAG 5000

#define EQUIPMENT_NUMBER_SHIELD 0

enum class ObjectType { MORNINGSTAR, AXE, DAGGER, BLUECROSS,  SHIELD, HOURGLASS, WATER, MASTERKEY, ORB,
	CHEST, WHITEBAG, BLUEBAG/*, FIRE*/ };

class Object : public Entity
{
public:
	AppStatus Initialise();
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	int Equip() const;

private:
	ObjectType type;
};

