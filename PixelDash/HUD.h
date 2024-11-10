#ifndef HUD_H
#define HUD_H

#include <vector>
#include "PixelSprite.h"

#pragma once

class HUD
{
public:
	static HUD& getInstance();
	static void init(olc::PixelGameEngine& pge);
	virtual ~HUD();
	void Draw();
	void Update(float);
	void decrLife() { m_nLife = std::max(m_nLife - 1, 0); };
	void incrLife() { m_nLife = std::min(m_nLife + 1, m_nTotalLife); };
	void incScore(int value) { m_nScore += value; };
	void resetScore() { m_nScore = 0; };

private:
	HUD();
	int m_nTotalLife = 3;
	int m_nLife = 3;
	int m_nScore = 0;
	//olc::PixelGameEngine& pge;
	PixelSprite* liveBar = nullptr;
	PixelSprite* smallDiamond = nullptr;
	PixelSprite* numberOne = nullptr;
	PixelSprite* numberTwo = nullptr;
	std::vector <PixelSprite*> lifeHearts;
	static HUD* instance;
	olc::PixelGameEngine* pge = nullptr; // Pointer to the game engine
};

#endif // HUD_H

