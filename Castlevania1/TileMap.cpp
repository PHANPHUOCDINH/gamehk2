#include "TileMap.h"
#define TILE_TEX 1000

TileMap::TileMap(int id_tex, LPCWSTR mapfile)
{
	this->mapFile = mapfile;
	this->id_tex = id_tex;
}


void TileMap::LoadTileSet() {
	string path = "resource//map1_mat.txt";
	fstream fs(path.c_str());
	int tiles;
	fs >> tiles >> row >> column;
	mapMatrix = new int*[row];
	for (int i = 0; i < row; i++)
	{
		mapMatrix[i] = new int[column];
		for (int j = 0; j < column; j++)
		{
			fs >> mapMatrix[i][j];
			DebugOut(L"Matrix %d", mapMatrix[i][j]);
		}
	}
	CTextures * ctexture = CTextures::GetInstance();
	LPDIRECT3DTEXTURE9 texTileMap = ctexture->Get(id_tex); //map 1 : 40

	CSprites * sprites = CSprites::GetInstance();
	for (int j = 0; j < tiles; j++) {
			int num = TILE_TEX+j;
			sprites->Add(num, j * 16, 0, (j + 1) * 16, 16, texTileMap);
			DebugOut(L"id %d left %d top %d right %d bottom %d \n", num, j * 16, 0, (j + 1) * 16, 16);
	}
}

void TileMap::Render() {
	CSprites * sprites = CSprites::GetInstance();
	float camposX;
	float camposY;
	CGame::GetInstance()->GetCamPos(camposX, camposY);
	camposX = camposX / 16;
	camposY = camposY / 16;
	float camposX2 = camposX + (SCREEN_WIDTH / 16);
	float camposY2 = camposY + (SCREEN_HEIGHT / 16);
	if (camposX < 0) {
		camposX = 0;
	}
	if (camposY < 0) {
		camposY = 0;
	}

}

void TileMap::GetMapSize(int& width, int& height) {
	width = column * MAP_TILE_SIZE;
	height = row * MAP_TILE_SIZE;
}

void TileMap::Update() {
	float camposX;
	float camposY;
	CGame::GetInstance()->GetCamPos(camposX, camposY);
	leftFrame = camposX / MAP_TILE_SIZE;
	rightFrame = leftFrame + 16;
}

TileMap::~TileMap()
{
}
