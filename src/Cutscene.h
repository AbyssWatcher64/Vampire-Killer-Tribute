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
	void UpdateCutscene();
	void Render();
	void Release();
	bool cutsceneFinished;
private:
	unsigned int timer;
	Player* player;
	Sprite* bat;
	Sprite* bat2;
	const Texture2D* cloud;
	const Texture2D* img_intro;
	const Texture2D* img_bat;
	float posx;
	int posxbat;
	int posybat;
	int posxbat2;
	int posybat2;
};
