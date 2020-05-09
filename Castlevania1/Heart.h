#pragma once
#include "Item.h"
#define BBOX_HEART_WIDTH 16
#define BBOX_HEART_HEIGHT 16
#define HEART_ANI_IDLE 0
class Heart :
	public Item
{
public:
	Heart();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	~Heart();
};

