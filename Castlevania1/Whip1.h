#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
//26 13
#define SPRITE_ID_START 41000

#define WHIP_BBOX_WIDTH_
#define WHIP_BBOX_HEIGHT 8


#define WHIP_ANI_0_LEFT 0
#define WHIP_ANI_0_RIGHT 1
#define WHIP_ANI_1_LEFT 2
#define WHIP_ANI_1_RIGHT 3


class Whip1 : public CGameObject
{
private:
	
public:
	Whip1();
	void atk(int dir);
	void Update();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	~Whip1();
};

