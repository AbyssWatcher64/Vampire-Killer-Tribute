#include "BlackLeopard.h"
#include "Sprite.h"

// Leopards do not have spawners, instead they are spawned by map
BlackLeopard::BlackLeopard(const Point& p, int width, int height, int frame_width, int frame_height, Look look) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = BlackLeopardState::STANDING;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::BLACKLEOPARD;
	this->look = look;
}
BlackLeopard::~BlackLeopard()
{
}
AppStatus BlackLeopard::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = BLACKLEOPARD_FRAME_SIZE_WIDTH;
	const int h = BLACKLEOPARD_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for leopard sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)BlackLeopardAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BlackLeopardAnim::STANDING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlackLeopardAnim::STANDING_LEFT, { (float)0 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)BlackLeopardAnim::STANDING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlackLeopardAnim::STANDING_RIGHT, { (float)0 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)BlackLeopardAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlackLeopardAnim::JUMPING_LEFT, { (float)2 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)BlackLeopardAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlackLeopardAnim::JUMPING_RIGHT, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)BlackLeopardAnim::RUNNING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlackLeopardAnim::RUNNING_LEFT, { (float)1 * n, 4 * h, -n, h });
	sprite->AddKeyFrame((int)BlackLeopardAnim::RUNNING_LEFT, { (float)2 * n, 4 * h, -n, h });
	sprite->AddKeyFrame((int)BlackLeopardAnim::RUNNING_LEFT, { (float)3 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)BlackLeopardAnim::RUNNING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlackLeopardAnim::RUNNING_RIGHT, { (float)1 * n, 4 * h, -n, h });
	sprite->AddKeyFrame((int)BlackLeopardAnim::RUNNING_RIGHT, { (float)2 * n, 4 * h, -n, h });
	sprite->AddKeyFrame((int)BlackLeopardAnim::RUNNING_RIGHT, { (float)3 * n, 4 * h, -n, h });

	if (look == Look::LEFT)        sprite->SetAnimation((int)BlackLeopardAnim::STANDING_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)BlackLeopardAnim::STANDING_RIGHT);

	visibility_area = area;

	//InitPattern();

	return AppStatus::OK;
}
//void BlackLeopard::InitPattern()
//{
//	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
//	//walking animation, resulting in smoother transitions and preventing the animation
//	//from appearing rushed or incomplete
//	const int n = BLACKLEOPARD_ANIM_DELAY * 3;
//
//	if (look == Look::LEFT)
//	{
//		pattern.push_back({ {-BLACKLEOPARD_SPEED, 0}, /*n*/0, (int)BlackLeopardAnim::STANDING_LEFT });
//
//	}
//	else
//		pattern.push_back({ {BLACKLEOPARD_SPEED, 0}, /*n*/0, (int)BlackLeopardAnim::STANDING_RIGHT });
//
//	current_step = 0;
//	current_frames = 0;
//}
bool BlackLeopard::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	if (state == BlackLeopardState::STANDING)
	{
		//state = BlackLeopardState::RUNNING;

	}
	else if (state == BlackLeopardState::RUNNING)
	{
		MoveX();
		if (look == Look::LEFT)
		{
			sprite->SetAnimation((int)BlackLeopardAnim::RUNNING_LEFT);
		}
		else
		{
			sprite->SetAnimation((int)BlackLeopardAnim::RUNNING_RIGHT);
		}
	}

	sprite->Update();

	//current_frames++;

	//if (current_frames == pattern[current_step].frames)
	//{
	//	current_step++;
	//	current_step %= pattern.size();
	//	current_frames = 0;

	//	anim_id = pattern[current_step].anim;
	//	sprite->SetAnimation(anim_id);
	//	UpdateLook(anim_id);
	//}

	//if (pos.x == -10)
	//	alive = false;
	//pos.x = 299;
//delete this;	

	//else if (pos.x == 300)
	//	alive = false
		//pos.x = -9;


	
	MoveY();




	return shoot;
}
void BlackLeopard::MoveX()
{
	if (look == Look::LEFT)
		pos.x -= BLACKLEOPARD_SPEED;
	else
		pos.x += BLACKLEOPARD_SPEED;
	

	//AABB box;
	//pos += pattern[current_step].speed;

}
void BlackLeopard::MoveY()
{
	//pos.y += BLACKLEOPARD_SPEED * 2;
	//AABB box, prev_box;

	//box = GetHitbox();
	//int prev_y;

	// For this reason this prevents zombie from going through the floor
	// even though it has nothing inside...
	//if (map->TestCollisionGround(box, &pos.y))
	//{

	//}
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
void BlackLeopard::UpdateLook(int anim_id)
{
	BlackLeopardAnim anim = (BlackLeopardAnim)anim_id;
	look = anim == BlackLeopardAnim::STANDING_LEFT ? Look::LEFT : Look::RIGHT;
}
void BlackLeopard::GetShootingPosDir(Point* p, Point* d) const
{
}
