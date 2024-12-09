#ifndef LEVEL_H
#define LEVEL_H
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm> 
#include "olcPixelGameEngine.h"
#include "GameConfig.h"
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
#include "TimedSprite.h"
#include "Cannon.h"

#pragma once

class Enemy;

class Level
{

private:
	Level();
	float fOffsetX;
	float fOffsetY;
	float fTotalTime = 0.0f;
	std::wstring sLevel;
	std::wstring sDecoration;
	std::unordered_set<wchar_t> moveAbleTiles;
	std::unordered_map<wchar_t, std::pair<int, int>> tileOffsets;
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteDoor = nullptr;
	PixelSprite* sDoorOpen = nullptr;
	std::unordered_map<wchar_t, std::vector<std::pair<PixelSprite*, std::pair<int, int>>>> pixelSprites;
	std::priority_queue<TimedSprite*> spriteQueue;
	std::list<TimedSprite*> activeSprites;;
	//Enemy *enemy = nullptr;
	static Level* instance;          // Singleton instance
	olc::PixelGameEngine* pge = nullptr; // Pointer to the game engine
	Enemy *enemy;
	Cannon* cannon;


public:
	static Level& getInstance();  // To retrieve instance after Init
	static void Init(olc::PixelGameEngine& pge);

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
	bool isMoveable(float x, float y);

	bool isDoor(float x, float y);
	void openDoor();
	PixelSprite* checkCollisionWithDecorations(const Rect& playerHitbox);
	PixelSprite* checkCollisionWithEnemies(const Rect& playerRect);
	PixelSprite* checkCollisionWithProjectiles(const Rect& playerRect);
	void removeDecoration(PixelSprite* decoration);
	void removeDecoration(wchar_t c, PixelSprite* decoration);
	float getOffsetX() { return fOffsetX; };
	float getOffsetY() { return fOffsetY; };

	float setOffsetX(float offsetX) { fOffsetX = offsetX; };
	float setOffsetY(float offsetY) { fOffsetY = offsetY; };
	bool isPlatform(int x, int y);
	bool isPlatform(float x, float y);
	float getTotalTime() { return fTotalTime; };
	void addTimedSprite(TimedSprite* ts);
	void HandleTimedSprites(float fElapsedTime);
};
#endif // LEVEL_H


