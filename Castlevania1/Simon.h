#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Whip1.h"
#define SIMON_WALKING_SPEED		0.1f 

#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_STAND_HIT		500
#define SIMON_STATE_STAND_HIT_LEFT		600
#define SIMON_STATE_DUCK			700
#define SIMON_STATE_HIT				800

#define SIMON_ANI_IDLE_LEFT		803
#define SIMON_ANI_IDLE_RIGHT			804
#define SIMON_ANI_WALKING_LEFT			805
#define SIMON_ANI_WALKING_RIGHT			806
#define SIMON_ANI_DUCK_LEFT		10
#define SIMON_ANI_DUCK_RIGHT	11
#define SIMON_ANI_JUMP_LEFT			4
#define SIMON_ANI_JUMP_RIGHT		5	
#define SIMON_ANI_DAMAGED_LEFT		6
#define SIMON_ANI_DAMAGED_RIGHT		7
#define SIMON_ANI_ATK_LEFT 815
#define SIMON_ANI_ATK_RIGHT 816

#define SIMON_ANI_DIE	20



#define SIMON_BIG_BBOX_WIDTH  16
#define SIMON_BIG_BBOX_HEIGHT 30

#define SIMON_BIG_ATK_BBOX_WIDTH  45
#define SIMON_BIG_ATK_BBOX_HEIGHT 30

#define SIMON_UNTOUCHABLE_TIME 5000


class CSimon : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;
	int onAir;
	int isJumping;
	Whip1* whip1;
	//SubWeapon* subweapon;
public:
	CSimon() : CGameObject()
	{
		SetState(SIMON_STATE_IDLE);
		onAir = 0;
		isJumping = 0;
		untouchable = 0;
		whip1 = new Whip1();
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void atk();
	void subatk();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};