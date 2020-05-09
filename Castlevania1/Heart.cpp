#include "Heart.h"
#include "Simon.h"

Heart::Heart()
{
	this->state = 0;
	this->atk_able = false;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);

	this->SetAnimationSet(ani_set);
	this->vy = -0.2f;
}

void Heart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BBOX_HEART_WIDTH;
	bottom = y + BBOX_HEART_HEIGHT;
}


void Heart::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {

	CGameObject::Update(dt);

	// Simple fall down
	vy += 0.002f * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}

}
void Heart::Render()
{
	if (state != OBJ_DIE)
		animation_set->at(HEART_ANI_IDLE)->Render(x, y);
}
Heart::~Heart()
{
}
