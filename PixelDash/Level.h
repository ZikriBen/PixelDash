#ifndef LEVEL_H
#define LEVEL_H
#include <string>
#include <unordered_map>
#include "olcPixelGameEngine.h"
#include <unordered_set>
#include "PixelSprite.h"
#include "Box.h"
#include "Coin.h"
#include "Heart.h"
#include "HUD.h"


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
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteDoor = nullptr;
	PixelSprite* sDoorOpen = nullptr;
	std::unordered_map<wchar_t, std::vector<std::pair<PixelSprite*, std::pair<float, float>>>> pixelSprites;
	HUD* hud = nullptr;
	//Enemy *enemy = nullptr;
	static Level* instance;          // Singleton instance
	olc::PixelGameEngine* pge = nullptr; // Pointer to the game engine
	Level(int levelWidth, int levelHeight, int tileWidth, int tileHeight);


public:
	static Level& getInstance();  // To retrieve instance after Init
	static void Init(olc::PixelGameEngine& pge, int levelWidth, int levelHeight, int tileWidth, int tileHeight);

	// Delete copy and assignment operators to enforce singleton
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

	void Init();
	void Update(float fElapsedTime);
	wchar_t GetTile(int x, int y);
	void SetTile(int x, int y, wchar_t c);
	void Draw(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY);

	const std::unordered_set<wchar_t> getMoveable() { return moveAbleTiles; }
	bool isMoveable(int x, int y);

	bool isDoor(float x, float y);
	void openDoor();
	PixelSprite* checkCollisionWithDecorations(const Rect& playerHitbox);
	void removeDecoration(PixelSprite* decoration);
	HUD* getHUD() { return hud; }
};
#endif // LEVEL_H


