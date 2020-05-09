#pragma once
#include "Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "Game.h"

#define MAP_TILE_SIZE 16
#include <fstream>
class TileMap
{
private:
	int** mapMatrix;
	int row;
	int column;
	LPCWSTR mapFile;
	int mapWidth, mapHeight;
	int tileWidth, tileHeight;
	int titlesetId;
	int id_tex;
	int leftFrame; //chứa chỉ số cột trái của background
	int rightFrame; //chứa chỉ số cột phải của background
	int TILE_SPRITE = 10000;
	LPDIRECT3DTEXTURE9 tileMap;
	LPSPRITE backGroundSprite;
public:
	TileMap(int id_tex,LPCWSTR mapfile);
	void GetMapSize(int& width, int& height);
	void LoadTileSet();
	void Update();
	void Render();
	~TileMap();
};

