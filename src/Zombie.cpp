#include "Zombie.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Zombie::Zombie(const Point& p, int width, int height, ZLook view) : Entity(p, width, height)
{
	look = view;
}
Zombie::~Zombie()
{
}
AppStatus Zombie::Initialise()
{
	int i;
	const int n = ZOMBIE_FRAME_SIZE;
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "img/enemies.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for zombie sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZombieAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZombieAnim::MOVING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
	{
		sprite->AddKeyFrame((int)ZombieAnim::MOVING_RIGHT, { (float)i*n, n * 2, n, n * 2 });
	}

	sprite->SetAnimationDelay((int)ZombieAnim::MOVING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
	{
		sprite->AddKeyFrame((int)ZombieAnim::MOVING_LEFT, { (float)i * n, n*2, -n, n*2});
	}

	return AppStatus::OK;
}
void Zombie::SetLook(ZLook view)
{
	look = view;
}
bool Zombie::IsLookingRight() const
{
	return look == ZLook::ZRIGHT;
}
void Zombie::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Zombie::WalkingLeft()
{
	SetDirection({ -ZOMBIE_SPEED, 0 });
	look = ZLook::ZLEFT;
	SetAnimation((int)ZombieAnim::MOVING_LEFT);
}
void Zombie::WalkingRight()
{
	SetDirection({ ZOMBIE_SPEED, 0 });
	look = ZLook::ZRIGHT;
	SetAnimation((int)ZombieAnim::MOVING_RIGHT);
}
void Zombie::Update()
{
	Entity::Update();
	
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	WalkingLeft();
}
void Zombie::DrawDebug(const Color& col) const
{
	int x, y;
	x = pos.x + ZOMBIE_FRAME_SIZE - ZOMBIE_PHYSICAL_WIDTH;
	y = pos.y;
	
	Entity::DrawDebug(x, y, width, height, col);
}
void Zombie::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ENEMIES);

	render->Release();
}