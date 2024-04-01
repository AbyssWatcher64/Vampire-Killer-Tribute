
#include "Entity.h"
#include <cmath>
#include "Globals.h"
/*#include "StaticImage.h"
#include "Sprite.h"*/

Entity::Entity(const Point& p, int w, int h) : pos(p), dir({ 0,0 }), width(w), height(h), render(nullptr)
{
}
Entity::~Entity()
{
	if (render != nullptr)
	{
		delete render;
		render = nullptr;
	}
}
void Entity::SetPos(const Point& p)
{
	pos = p;
}
const Point& Entity::GetPos() const
{
	return pos;
}
void Entity::SetDirection(const Point& d)
{
	dir = d;
}
void Entity::Update()
{
	pos += dir;
}
void Entity::Draw() const
{
	render->Draw(pos.x, pos.y);
}
void Entity::DrawTint(const Color& col) const
{
	render->DrawTint(pos.x, pos.y, col);
}
void Entity::DrawDebug(const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(pos.x, pos.y, width, height, c);
	render->DrawCorners(pos.x, pos.y, width, height);
}
void Entity::DrawDebug(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(x, y, w, h, c);
	render->DrawCorners(x, y, w, h);
}
