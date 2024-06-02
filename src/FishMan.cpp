#include "Fishman.h"
#include "Sprite.h"

Fishman::Fishman(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = FishmanState::ROAMING;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::FISHMAN;
	canWalk = false;
	hasSpawned = false;
	damage = 4;
}
Fishman::~Fishman()
{
}
AppStatus Fishman::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = FISHMAN_FRAME_SIZE_WIDTH;
	const int h = FISHMAN_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for fishman sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)FishmanAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)FishmanAnim::WALKING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)FishmanAnim::WALKING_RIGHT, { (float)0 * n, 5 * n, -n, h });
	sprite->AddKeyFrame((int)FishmanAnim::WALKING_RIGHT, { (float)1 * n, 5 * n, -n, h });

	sprite->SetAnimationDelay((int)FishmanAnim::WALKING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)FishmanAnim::WALKING_LEFT, { (float)0 * n, 5 * n, n, h });
	sprite->AddKeyFrame((int)FishmanAnim::WALKING_LEFT, { (float)1 * n, 5 * n, n, h });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)FishmanAnim::WALKING_LEFT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)FishmanAnim::WALKING_RIGHT);

	visibility_area = area;

	InitPattern();
	timer = 0;
	data.PlaySFX(14);
	return AppStatus::OK;
}
void Fishman::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = FISHMAN_ANIM_DELAY * 3;

	if (look == Look::LEFT)
	{
		pattern.push_back({ {-FISHMAN_SPEED, 0}, /*n*/0, (int)FishmanAnim::WALKING_LEFT });

	}
	else 
		pattern.push_back({ {FISHMAN_SPEED, 0}, /*n*/0, (int)FishmanAnim::WALKING_RIGHT });

	current_step = 0;
	current_frames = 0;
}
bool Fishman::Update(const AABB& box)
{
	timer++;
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
	
	FishManInitialFalling();
	MoveY();
	
	if (canWalk)
	{
		MoveX();
		sprite->Update();
	}

	return shoot;
}

void Fishman::MoveX()
{
	AABB box;
	pos += pattern[current_step].speed;
}
void Fishman::MoveY()
{
	pos.y += FISHMAN_SPEED;
	AABB box, prev_box;

	box = GetHitbox();
	int prev_y;

	// For this reason this prevents the fish from going through the floor
	// even though it has nothing inside...
	if (map->TestCollisionGround(box, &pos.y))
	{
		canWalk = true;
		
	}
	else
	{
		canWalk = false;
	}
		
	// TODO: Fix movement.
	
}
void Fishman::FishManInitialFalling()
{
	if (!hasSpawned)
	{
		dir.y = -FISHMAN_JUMP_FORCE;
		hasSpawned = true;
	}
	AABB box, prev_box;
	int prev_y;

	
	prev_y = pos.y;
	prev_box = GetHitbox();

	pos.y += dir.y;
	if (timer % 3 == 0)
		dir.y += GRAVITY_FORCE;

	//Is the jump finished?
	if (dir.y > FISHMAN_JUMP_FORCE)
	{
		dir.y = FISHMAN_SPEED;
	}

}

void Fishman::UpdateLook(int anim_id)
{
	FishmanAnim anim = (FishmanAnim)anim_id;
	look =	anim == FishmanAnim::WALKING_LEFT ? Look::LEFT : Look::RIGHT;
}
void Fishman::GetShootingPosDir(Point* p, Point* d) const
{
}
