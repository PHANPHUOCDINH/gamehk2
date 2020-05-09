#include "Whip1.h"
#include "Textures.h"
#include "Heart.h"
Whip1::Whip1()
{

}

void Whip1::Update()
{
	
}


void Whip1::atk(int dir) {
	
}

void Whip1::GetBoundingBox(float &left, float &top, float &right, float &bottom) {
	left = x;
	top = y;
	right = x + WHIP_BBOX_WIDTH_;
	bottom=
}

void Whip1::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {
		CGameObject::Update(dt, coObjects);
		vector<LPCOLLISIONEVENT> coEvents;
		CalcPotentialCollisions(coObjects, coEvents);
		float wl, wt, wr, wb;
		this->GetBoundingBox(wl, wt, wr, wb);
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->state != OBJ_DIE) {
				float l, t, r, b;
				coObjects->at(i)->GetBoundingBox(l, t, r, b);
				if (l > wl && l < wr) {
					if (!(b < wt || t > wb)) {
						coObjects->at(i)->SetState(OBJ_DIE);
						//Item::CreateItem(coObjects->at(i)->x, coObjects->at(i)->y);
					}
				}
			}


		}
}

void Whip1::Render() {
		CSprites * sprites = CSprites::GetInstance();
		//if left			srprite bắt đầu| level (100) | trái| +1 nữa ra phải 
		int spriteIndex = SPRITE_ID_START + 100 * level + 1 + dir;

		spriteIndex += 10 * aniCount;
		sprites->Get(spriteIndex)->Draw(x, y);
		//RenderBoundingBox();
		//DebugOut(L"draw whips ani %d \n", spriteIndex);
		DWORD now = GetTickCount();
		DWORD t = 80;
		if (now - lastFrameTime > t)
		{
			aniCount++;
			lastFrameTime = now;

			if (aniCount == 3) {
				aniCount = 0;
				isAtk = false;
				return;
			}
		}

}

Whip1::~Whip1()
{
}
