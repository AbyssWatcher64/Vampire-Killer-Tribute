#include "Zombie.h"
#include "Sprite.h"

Zombie::Zombie(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = ZombieState::ROAMING;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::ZOMBIE;
	damage = 2;
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
		LOG("Failed to allocate memory for zombie sprite");
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

	if (look == Look::LEFT)
	{
		pattern.push_back({ {-ZOMBIE_SPEED, 0}, /*n*/0, (int)ZombieAnim::WALKING_LEFT });

	}
	else 
		pattern.push_back({ {ZOMBIE_SPEED, 0}, /*n*/0, (int)ZombieAnim::WALKING_RIGHT });

	current_step = 0;
	current_frames = 0;
}
bool Zombie::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	
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

	//if (pos.x == -10)
	//	alive = false;
	//pos.x = 299;
//delete this;	

	//else if (pos.x == 300)
	//	alive = false
		//pos.x = -9;
		

	MoveX();
	MoveY();

	sprite->Update();

	return shoot;
}
void Zombie::MoveX()
{
	AABB box;
	pos += pattern[current_step].speed;

}
void Zombie::MoveY()
{
	pos.y += ZOMBIE_SPEED * 2;
	AABB box, prev_box;

	box = GetHitbox();
	int prev_y;

	// For this reason this prevents zombie from going through the floor
	// even though it has nothing inside...
	if (map->TestCollisionGround(box, &pos.y))
	{

	}
	//{

	//}

		
		//A ground collision occurs if we were not in a collision state previously.
		//This prevents scenarios where, after levitating due to a previous jump, we found
		//ourselves inside a tile, and the entity would otherwise be placed above the tile,
		//crossing it.
		//if (!map->TestCollisionGround(prev_box, &prev_y) &&
		//	map->TestCollisionGround(box, &pos.y))
		//{
		//	dir = { 0,0 };
		//}
	
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
