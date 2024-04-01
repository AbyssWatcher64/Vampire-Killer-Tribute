
#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"

class Entity
{
public:
	Entity(const Point& p, int w, int h);
	virtual ~Entity();

	void SetPos(const Point& p);
	const Point& GetPos() const;

	void SetDirection(const Point& d);
	void Update();

	void Draw() const;
	void DrawTint(const Color& col) const;
	void DrawDebug(const Color& col) const;
	void DrawDebug(int x, int y, int w, int h, const Color& col) const;

protected:
	Point pos, dir;
	int width, height;
	RenderComponent *render;
};