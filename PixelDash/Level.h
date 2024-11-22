#ifndef LEVEL_H
#define LEVEL_H
#include <string>
#include <unordered_map>
#include "olcPixelGameEngine.h"
#include <unordered_set>
#include "PixelSprite.h"
#include "Box.h"
#include "Coin.h"
#include "Window.h"
#include "Platform.h"
#include "Flag.h"
#include "Heart.h"
#include "HUD.h"
#include "Enemy.h"

#pragma once

class Enemy;

class Level
{

private:
	int nLevelWidth;
	int nLevelHeight;
	int nTileWidth;
	int nTileHeight;
	float fOffsetX;
	float fOffsetY;
	std::wstring sLevel;
	std::wstring sDecoration;
	std::unordered_set<wchar_t> moveAbleTiles;
	std::unordered_map<wchar_t, std::pair<int, int>> tileOffsets;
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteDoor = nullptr;
	PixelSprite* sDoorOpen = nullptr;
	std::unordered_map<wchar_t, std::vector<std::pair<PixelSprite*, std::pair<float, float>>>> pixelSprites;
	//Enemy *enemy = nullptr;
	static Level* instance;          // Singleton instance
	olc::PixelGameEngine* pge = nullptr; // Pointer to the game engine
	Level(int levelWidth, int levelHeight, int tileWidth, int tileHeight);
	Enemy *enemy;


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
	bool isMoveable(int x, int y, float velY, Rect playerRect);
	bool isMoveable(int x, int y, float velY);

	bool isDoor(float x, float y);
	void openDoor();
	PixelSprite* checkCollisionWithDecorations(const Rect& playerHitbox);
	PixelSprite* checkCollisionWithEnemies(const Rect& playerRect);
	void removeDecoration(PixelSprite* decoration);
	float getOffsetX() { return fOffsetX; };
	float getOffsetY() { return fOffsetY; };

	float setOffsetX(float offsetX) { fOffsetX = offsetX; };
	float setOffsetY(float offsetY) { fOffsetY = offsetY; };
	bool isPlatform(int x, int y);
};
#endif // LEVEL_H


