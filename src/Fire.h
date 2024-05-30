#pragma once
#include "Entity.h"

#define FIRE_PHYSICAL_SIZE	14
#define FIRE_FRAME_SIZE		16

enum class FireType { PYRE, CANDLE };

class Fire : public Entity
{
public:
	AppStatus Initialise();
	Fire(const Point& p, FireType t);
	~Fire();
	void DrawDebug(const Color& col) const;
	int ObjectNum() const;

private:
	FireType type;
};