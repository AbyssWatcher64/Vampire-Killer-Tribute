#include "Door.h"
#include "Sprite.h"

Door::Door(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = DoorState::STATIC;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::DOOR;
	width = 32;
	height = 16;
	doorFrames = 0;
	isSolid = true;
}
Door::~Door()
{
}
AppStatus Door::Initialise(Look look, const AABB& area)
{
	const int n = DOOR_FRAME_SIZE_WIDTH - 4;
	const int h = DOOR_FRAME_SIZE_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_TILES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for door sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)DoorAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)DoorAnim::STATIC, ANIM_DELAY);
	sprite->AddKeyFrame((int)DoorAnim::STATIC, { (float)8 * 16, 8 * 16, n, h });

	sprite->SetAnimationDelay((int)DoorAnim::OPENING, ANIM_DELAY);
	for (float i = 0; i < 49; i++)
	{
		sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + i, n, h - i });
	}
	/*sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 1, n, h - 1 });
	sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 2, n, h - 2 });
	sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 3, n, h - 3 });
	sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 4, n, h - 4 });
	sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 5, n, h - 5 });
	sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 6, n, h - 6 });
	sprite->AddKeyFrame((int)DoorAnim::OPENING, { (float)8 * 16, 8 * 16 + 7, n, h - 7 });*/
	this->look = look;
	sprite->SetAnimation((int)DoorAnim::STATIC);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}
void Door::InitPattern()
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete
	const int n = DOOR_ANIM_DELAY;

	pattern.push_back({ {DOOR_SPEED, 1},  n, (int)DoorAnim::STATIC });


	current_step = 0;
	current_frames = 0;
}
bool Door::Update(const AABB& box)
{
	doorFrames++;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;
	
	if (state == DoorState::STATIC && playerHasWhiteKey && openDoor)
	{
		state = DoorState::OPENING;
		sprite->SetAnimation((int)DoorAnim::OPENING);
		doorFrames = 0;
	}
	if (state == DoorState::OPENING)
	{
		if (doorFrames == 192)
			alive = false;
	}
	current_frames++;

	if (current_frames == pattern[current_step].frames)
	{
		current_step++;
		current_step %= pattern.size();
		current_frames = 0;

		anim_id = pattern[current_step].anim;
		/*sprite->SetAnimation(anim_id);*/
		UpdateLook(anim_id);
	}

	sprite->Update();

	return shoot;
}

void Door::UpdateLook(int anim_id)
{
	DoorAnim anim = (DoorAnim)anim_id;
	look = anim == DoorAnim::STATIC ? Look::LEFT : Look::RIGHT;
}
void Door::GetShootingPosDir(Point* p, Point* d) const
{
}
