#pragma once
#include "Entity.h"

#define WHIP_FRAME_SIZE	0
#define KNIFE_FRAME_SIZE	16
#define AXE_FRAME_WIDTH		32
#define AXE_FRAME_HEIGHT	16
#define AXE_FRAME_SIZE		32


#define LOGICAL_WHIP_WIDTH_SIZE				32
#define LOGICAL_WHIP_HEIGHT_SIZE			12
#define LOGICAL_MORNINGSTAR_WIDTH_SIZE		40
#define LOGICAL_MORNINGSTAR_HEIGHT_SIZE		12


enum class WeaponType
{
	WHIP, MORNINGSTAR, AXE, DAGGER
};

class Weapon : public Entity
{
public:
	AppStatus Initialise();
	Weapon(const Point& p, WeaponType t);
	~Weapon();

	void DrawDebug(const Color& col) const;

private:
	WeaponType type;
	int weaponLogicalWidth;
	int weaponLogicalHeight;
};