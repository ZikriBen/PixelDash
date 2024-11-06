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
	void decrLife() {};
	void incrLife() {};
	void incScore(int value) { m_nScore += value; };
	void resetScore() { m_nScore = 0; };

private:
	int m_nLife;
	int m_nScore;
	olc::PixelGameEngine& pge;
	PixelSprite* liveBar = nullptr;
	std::vector <PixelSprite*> lifeHearts;
};

#endif // HUD_H

