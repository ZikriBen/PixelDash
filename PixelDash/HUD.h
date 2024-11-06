#ifndef HUD_H
#define HUD_H

#include <vector>
#include "PixelSprite.h"

#pragma once

class HUD
{
public:
	HUD(olc::PixelGameEngine& pge) : pge(pge) {};
	virtual ~HUD();
	void init();
	void Draw();
	void Update(float);
	void decrLife() { m_nLife = std::max(m_nLife - 1, 0); };
	void incrLife() { m_nLife = std::min(m_nLife + 1, m_nTotalLife); };
	void incScore(int value) { m_nScore += value; };
	void resetScore() { m_nScore = 0; };

private:
	int m_nTotalLife = 3;
	int m_nLife = 3;
	int m_nScore = 0;
	olc::PixelGameEngine& pge;
	PixelSprite* liveBar = nullptr;
	PixelSprite* smallDiamond = nullptr;
	PixelSprite* numberOne = nullptr;
	PixelSprite* numberTwo = nullptr;
	std::vector <PixelSprite*> lifeHearts;
};

#endif // HUD_H

