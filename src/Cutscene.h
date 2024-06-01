#pragma once
#include <raylib.h>
#include "Player.h"
#include "Sprite.h"

class Cutscene
{
public:
	Cutscene();
	~Cutscene();

	AppStatus Init();
	AppStatus LoadResources();
	void UpdateCutscene();
	void Render();
	void Release();
	bool cutsceneFinished;
private:
	unsigned int timer;
	Player* player;
	Sprite* bat;
	const Texture2D* cloud;
	const Texture2D* img_intro;
	float posx;
	
};
