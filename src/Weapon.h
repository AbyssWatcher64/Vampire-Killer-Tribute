//#pragma once
//#include "Entity.h"
//
//#define WHIP_FRAME_WIDTH	64
//#define WHIP_FRAME_HEIGHT	16
//#define MORNINGSTAR_FRAME_WIDTH		64
//#define MORNINGSTAR_FRAME_HEIGHT	16
//#define KNIFE_FRAME_WIDTH	16
//#define KNIFE_FRAME_HEIGHT	16
//#define AXE_FRAME_WIDTH		32
//#define AXE_FRAME_HEIGHT	16
//#define AXE_FRAME_SIZE		32
//
//
//#define LOGICAL_WHIP_WIDTH_SIZE				32
//#define LOGICAL_WHIP_HEIGHT_SIZE			12
//#define LOGICAL_MORNINGSTAR_WIDTH_SIZE		40
//#define LOGICAL_MORNINGSTAR_HEIGHT_SIZE		12
//
//
//enum class WeaponType
//{
//	WHIP, MORNINGSTAR, AXE, DAGGER
//};
//
//class Weapon : public Entity
//{
//public:
//	Weapon(const Point& p, WeaponType t);
//	~Weapon();
//	void Update();
//	void Release();
//	void DrawDebug(const Color& col) const;
//
//	void Init(const Point& p, const Point& dir);
//	bool IsMovingLeft() const;
//	bool IsMovingRight() const;
//	void DrawDebug(const Color& col) const;
//
//private:
//	WeaponType type;
//	int weaponLogicalWidth;
//	int weaponLogicalHeight;
//};