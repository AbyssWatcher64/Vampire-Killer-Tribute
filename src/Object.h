#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

#define POINTS_BLUEBAG 1000
#define POINTS_WHITEBAG 5000

#define EQUIPMENT_NUMBER_SHIELD		0
#define EQUPMENT_NUMBER_MORNINGSTAR 100
#define EQUPMENT_NUMBER_AXE			200
#define EQUPMENT_NUMBER_DAGGER		300
#define EQUPMENT_NUMBER_CROSS		400

#define EQUIPMENT_WINNING_ORB	1000

enum class ObjectType {
	MORNINGSTAR, AXE, DAGGER, BLUECROSS, HOURGLASS, HOLYWATER, MASTERKEY, ORB,
	CHEST, YELLOWKEY, WHITEKEY, HEART, BIGHEART, SMALLORB, WHITEBAG, BLUEBAG,
	
	BOOTS, REDSHIELD, SHIELD, RING, REDBOOK, FEATHER, WHITECROSS, YELLOWCROSS, MAP
	/*, FIRE*/ 
};

class Object : public Entity
{
public:
	AppStatus Initialise();
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	int Equip() const;
	int ObjectNum() const;


private:
	ObjectType type;
};

