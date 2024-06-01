#include "DestroyableBlocks.h"
#include "Sprite.h"

DestroyableBlock::DestroyableBlock(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = DestroyableBlockState::STATIC;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::BLOCKS;
	width = 32;
	height = 16;
}
DestroyableBlock::~DestroyableBlock()
{
}
AppStatus DestroyableBlock::Initialise(Look look, const AABB& area)
{
	const int n = BLOCKS_FRAME_SIZE;
	const int h = BLOCKS_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_TILES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for block sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)DestroyableBlockAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)DestroyableBlockAnim::STATIC, ANIM_DELAY + 8);
	/*for (i = 1; i < 2; ++i)
		sprite->AddKeyFrame((int)BatAnim::FLYING_RIGHT, { (float)i * n, 7 * n, -n, h });*/
	sprite->AddKeyFrame((int)DestroyableBlockAnim::STATIC, { (float)4 * (n/2), 9 * (h/2), n, h });

	this->look = look;
	sprite->SetAnimation((int)DestroyableBlockAnim::STATIC);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void DestroyableBlock::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = BLOCKS_ANIM_DELAY;

	pattern.push_back({ {BLOCKS_SPEED, 1},  n, (int)DestroyableBlockAnim::STATIC });


	current_step = 0;
	current_frames = 0;
}
bool DestroyableBlock::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;


	current_frames++;
	//pos += pattern[current_step].speed;

	if (current_frames == pattern[current_step].frames)
	{
		current_step++;
		current_step %= pattern.size();
		current_frames = 0;

		anim_id = pattern[current_step].anim;
		/*sprite->SetAnimation(anim_id);*/
		UpdateLook(anim_id);
	}

	//if (pos.x == -10)
	//	alive = false;
	//pos.x = 299;
//delete this;	

	//else if (pos.x == 300)
	//	alive = false
		//pos.x = -9;


	sprite->Update();

	return shoot;
}

void DestroyableBlock::UpdateLook(int anim_id)
{
	DestroyableBlockAnim anim = (DestroyableBlockAnim)anim_id;
	look = anim == DestroyableBlockAnim::STATIC ? Look::LEFT : Look::RIGHT;
}
void DestroyableBlock::GetShootingPosDir(Point* p, Point* d) const
{
}
