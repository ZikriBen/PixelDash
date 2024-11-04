#ifndef LEVEL_H
#define LEVEL_H
#include <string>
#include <unordered_map>
#include "olcPixelGameEngine.h"
#include "PixelSprite.h"
#include <unordered_set>

#pragma once
class Level
{

private:
	int nLevelWidth;
	int nLevelHeight;
	int nTileWidth;
	int nTileHeight;
	std::wstring sLevel;
	std::wstring sDecoration;
	std::unordered_set<wchar_t> moveAbleTiles;
	std::unordered_map<wchar_t, std::pair<int, int>> tileOffsets;
	std::unordered_map<wchar_t, std::vector<std::pair<float, float>>> decorPos;
	olc::PixelGameEngine& pge;
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteDoor = nullptr;
	PixelSprite* sDoorOpen = nullptr;
	//std::unordered_map<wchar_t, std::pair<PixelSprite*, std::pair<float, float>>> pixelSprites;
	std::unordered_map<wchar_t, std::vector<std::pair<PixelSprite*, std::pair<float, float>>>> pixelSprites;

public:
	Level(olc::PixelGameEngine& pge, int levelWidth, int levelHeight, int tileWidth, int tileHeight);
	//Level(olc::PixelGameEngine& pge, olc::Sprite* spriteTiles, int nLevelWidth, int nLevelHeight, int tileWidth, int tileHeight, std::wstring level, std::wstring decoration, std::unordered_map<wchar_t, std::pair<int, int>> tileOffsets);
	void Build();
	void Update(float fElapsedTime);
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, wchar_t c);
	void Draw(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY);

	const std::unordered_set<wchar_t> getMoveable() { return moveAbleTiles; }
	bool isMoveable(int x, int y);

	bool isDoor(float x, float y);
	void openDoor();
};
#endif // LEVEL_H


