#include "FireLamp.h"
#include "Textures.h"
#include "Sprites.h"

FireLamp::FireLamp()
{
	

}


FireLamp::~FireLamp()
{
}

void FireLamp::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {


}

void FireLamp::Render() {
	if (state != LAMP_STATE_DESTROY)
		animation_set->at(LAMP_ANI_IDLE)->Render(x, y);
}

void  FireLamp::GetBoundingBox(float &left, float &top, float &right, float &bottom) {
	left = x;
	top = y;
	right = x + LAMP_BBOX_WIDTH;
	bottom = y + LAMP_BBOX_HEIGHT;
}