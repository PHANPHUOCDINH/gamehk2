#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Simon.h"
#include "Brick.h"
class Scene1 : public CScene
{
protected:
	CSimon *player;					// A play scene has to have player, right? 
	TileMap *map;
	vector<LPGAMEOBJECT> objects;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_Map(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	Scene1(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CSimon * GetPlayer() { return player; }
	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

