#include "Zombie.h"
#include "Sprite.h"

Zombie::Zombie(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = ZombieState::ROAMING;
	//map = nullptr;

	current_step = 0;
	current_frames = 0;
}
Zombie::~Zombie()
{
}
AppStatus Zombie::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = ZOMBIE_FRAME_SIZE_WIDTH;
	const int h = ZOMBIE_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for slime sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZombieAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_RIGHT, { (float)i * n, 2 * n, -n, h });
	sprite->AddKeyFrame((int)ZombieAnim::WALKING_RIGHT, { (float)0 * n, 2 * n, -n, h });

	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_LEFT, { (float)i * n, 2 * n, n, h });
	sprite->AddKeyFrame((int)ZombieAnim::WALKING_LEFT, { (float)0 * n, 2 * n, n, h });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)ZombieAnim::WALKING_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)ZombieAnim::WALKING_RIGHT);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void Zombie::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = ZOMBIE_ANIM_DELAY * 3;

	pattern.push_back({ {ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_RIGHT });
	pattern.push_back({ {-ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_LEFT });

	current_step = 0;
	current_frames = 0;
}
bool Zombie::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	pos += pattern[current_step].speed;
	current_frames++;

	if (current_frames == pattern[current_step].frames)
	{
		current_step++;
		current_step %= pattern.size();
		current_frames = 0;

		anim_id = pattern[current_step].anim;
		sprite->SetAnimation(anim_id);
		UpdateLook(anim_id);
	}

	MoveX();

	sprite->Update();

	return shoot;
}
void Zombie::MoveX()
{
	AABB box;


}
//void Zombie::SetTileMap(TileMap* tilemap)
//{
//	map = tilemap;
//}
void Zombie::UpdateLook(int anim_id)
{
	ZombieAnim anim = (ZombieAnim)anim_id;
	look =	anim == ZombieAnim::WALKING_LEFT ? Look::LEFT : Look::RIGHT;
}
void Zombie::GetShootingPosDir(Point* p, Point* d) const
{
}
