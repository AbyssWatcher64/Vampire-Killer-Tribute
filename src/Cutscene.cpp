#include "Cutscene.h"
#include <stdio.h>
#include "Globals.h"

Cutscene::Cutscene()
{
	player = nullptr;
	timer = 0;
	img_intro = nullptr;
	cloud = nullptr;
	bat = nullptr;
	bat2 = nullptr;
	cutsceneFinished = false;
	posx = WINDOW_WIDTH - 70;
	posxbat = 47;
	posybat = 96;
	posxbat2 = 140;
	posybat2 = 50;
}
Cutscene::~Cutscene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (bat != nullptr)
	{
		bat->Release();
		delete bat;
		bat = nullptr;
	}
	if (bat2 != nullptr)
	{
		bat2->Release();
		delete bat2;
		bat2 = nullptr;
	}
}
AppStatus Cutscene::Init()
{
	timer = 0;
	cutsceneFinished = false;
	posx = WINDOW_WIDTH - 70;
	posxbat = 47;
	posybat = 96;
	posxbat2 = 140;
	posybat2 = 50;
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	player->InitPattern();

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_INTRO, "img/misc.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_intro = data.GetTexture(Resource::IMG_INTRO);
	if (data.LoadTexture(Resource::IMG_CLOUD, "img/misc.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	cloud = data.GetTexture(Resource::IMG_CLOUD);

	const int n = 16;
	const int h = 16;
	/*if (data.LoadTexture(Resource::IMG_BAT, "img/misc.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}*/
	bat = new Sprite(data.GetTexture(Resource::IMG_MISC));
	if (bat == nullptr)
	{
		LOG("Failed to allocate memory for bat sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(bat);
	sprite->SetNumberAnimations(1);

	sprite->SetAnimationDelay(0, ANIM_DELAY);
	for (int i = 0; i < 20; i++)
	{
		sprite->AddKeyFrame(0, { (float)21 * n, 15 * n, n, h });
		sprite->AddKeyFrame(0, { (float)22 * n, 15 * n, n, h });
	}
	sprite->SetAnimation(0);

	bat2 = new Sprite(data.GetTexture(Resource::IMG_MISC));
	if (bat2 == nullptr)
	{
		LOG("Failed to allocate memory for bat2 sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite2 = dynamic_cast<Sprite*>(bat2);
	sprite2->SetNumberAnimations(1);

	sprite2->SetAnimationDelay(0, ANIM_DELAY);
	for (int i = 0; i < 20; i++)
	{
		sprite2->AddKeyFrame(0, { (float)21 * n, 15 * n, n, h });
		sprite2->AddKeyFrame(0, { (float)22 * n, 15 * n, n, h });
	}
	sprite2->SetAnimation(0);
	

	player->SetPos(Point(WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH, 199));
	return AppStatus::OK;
}
void Cutscene::UpdateCutscene()
{
	timer++;
	
	Point p1, p2;
	AABB hitbox;
	
	player->UpdatePattern();
	hitbox = player->GetHitbox();
	if ((timer % 10) == 0)
	{
		posx--;
		posxbat++;
		posxbat2--;
		
	}
	if ((timer % 20) == 0)
	{
		posybat--;
	}
	if (timer==440 || IsKeyPressed(KEY_SPACE))
	{
		timer = 0;
		cutsceneFinished = true;
		
	}
	
	Sprite* sprite = dynamic_cast<Sprite*>(bat);
	sprite->Update();
	Sprite* sprite2 = dynamic_cast<Sprite*>(bat2);
	sprite2->Update();
}
void Cutscene::Render()
{
	DrawTexturePro(*img_intro, { 8,246,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
	DrawTexturePro(*cloud, { 272,240,59,16 }, { posx,64,59,16 }, { 0,0 }, 0, WHITE);
	player->Draw();
	bat->Draw(posxbat, posybat);
	bat2->Draw(posxbat2, posybat2);
}
void Cutscene::Release()
{
	player->Release();
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_INTRO);
	data.ReleaseTexture(Resource::IMG_CLOUD);
	bat->Release();
	bat2->Release();
}
