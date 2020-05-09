#include <iostream>
#include <fstream>

#include "Scene1.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

#include "FireLamp.h"
#include "Heart.h"
using namespace std;

Scene1::Scene1(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAPSET 7

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_FIRELAMP	2
#define OBJECT_TYPE_HEART	4
#define OBJECT_TYPE_PORTAL	10
#define OBJECT_TYPE_BRICK 1
#define MAX_SCENE_LINE 1024


void Scene1::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"%d tokens", tokens.size());
	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void Scene1::_ParseSection_Map(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() !=2) return;
	wstring mappath = ToWSTR(tokens[1]);
	int texID = atoi(tokens[0].c_str());
	DebugOut(L"XXXXXXXX%s\n",mappath.c_str());
//	string mappath = tokens[1];
	map = new TileMap(texID,mappath.c_str());
	map->LoadTileSet();
}

void Scene1::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 8) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int ax = atoi(tokens[5].c_str());
	int ay = atoi(tokens[6].c_str());
	int texID = atoi(tokens[7].c_str());
	if (ax != 0) {
		DebugOut(L"");
	}
	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, ax, ay, tex);
}

void Scene1::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void Scene1::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void Scene1::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] Simon object was created before! ");
			return;
		}
		obj = new CSimon();
		player = (CSimon*)obj;
		break;
	case OBJECT_TYPE_FIRELAMP: obj = new FireLamp(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void Scene1::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	gametime = 0;
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAPSET; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_MAPSET: _ParseSection_Map(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	f.close();

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void Scene1::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	gametime += dt;
	

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CSimon *>(objects[i])) {
			objects[i]->Update(dt, &coObjects);
		}

	}
	player->Update(dt, &coObjects);

	

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	
	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

void Scene1::Render()
{
	map->Render();
	for(int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	player->Render();
	//DebugOut(L"%d Objects", player->animation_set->size());
	float x, y;
	player->GetPosition(x,y);
	DebugOut(L"X: %f Y: %f\n", x, y);
}

/*
	Unload current scene
*/
void Scene1::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CSimon *simon = ((Scene1*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	}
	
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	/*CSimon *simon = ((Scene1*)scence)->player;
	if (KeyCode == DIK_DOWN) {
		simon->SetStatus(SIMON_STA_STANDUP);
	}*/
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CSimon *simon = ((Scene1*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else
	if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	/*else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_DUCK);
	else if (game->IsKeyDown(DIK_UP))
		simon->SetState(SIMON_STATE_CLIMP);*/
	else
		simon->SetState(SIMON_STATE_IDLE);
}