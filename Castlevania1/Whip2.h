#pragma once
#include "GameObject.h"
#include "Item.h"
#define BBOX_WHIP_WIDTH 16
#define BBOX_WHIP_HEIGHT 16
#define WHIP_ANI_IDLE 0

class Whip2 : public Item
{
public:
	Whip2();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	~Whip2();
};

