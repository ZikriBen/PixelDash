#ifndef PIXEL_SPRITE_H
#define PIXEL_SPRITE_H

#include "olcPixelGameEngine.h"
#include "Rect.h"

#pragma once
class PixelSprite 
{
public:
	PixelSprite(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy);
	PixelSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY);
	virtual ~PixelSprite();
	virtual void Update(float);
	virtual void Draw();
	float getWidth() { return fWidth; };
	float getHeight() { return fHeight; };
	void setPosX(float newPos) { fPosX = newPos; };
	void setPosY(float newPos) { fPosY = newPos; };
	float getPosX() { return fPosX; };
	float getPosY() { return fPosY; };
	float getGraphicTimer() { return fGraphicTimer; }
	void setGraphicTimer(float newTimer) { fGraphicTimer = newTimer; }
	void incGraphicCounter() { iGraphicCounter++; }
	void setGraphicCounter(float newCounter) { iGraphicCounter = newCounter; }
	int getGraphicCounter() { return iGraphicCounter; }
	void setAnimation(bool animate) { bIsAnimate = animate; };
	void setLoop(bool loop) { bIsLoop = loop; };
	void setScale(int scale) { iScale = scale; };
	float getOffsetPosX() { return fPosOffsetX; };
	float getOffsetPosY() { return fPosOffsetY; };
	void setOffsetPosX(float offsetX) { fPosOffsetX = offsetX; };
	void setOffsetPosY(float offsetY) { fPosOffsetY = offsetY; };

	float getSprOffsetX() { return fOffsetX; };
	float getSprOffsetY() { return fOffsetY; };

	void setSprOffsetX(float offsetX) { fOffsetX = offsetX; };
	void setSprOffsetY(float offsetY) { fOffsetY = offsetY; };

	void setNumFrames(int n) { iNumFrames = n; };

	Rect getRect();

protected:
	olc::PixelGameEngine& pge;
	enum { RIGHT = 0, LEFT = 1 } eFacingDirection;

private:
	olc::Sprite* spr;
	int iNumFrames;
	float fFrameDuration;
	bool bIsAnimate;
	bool bIsLoop;
	float fPosX;
	float fPosY;
	float fOffsetX;
	float fOffsetY;
	float fPosOffsetX;
	float fPosOffsetY;
	float fWidth;
	float fHeight;
	int iScale;
	int iFlip;
	float fCurrentHealth;
	float fGraphicTimer;
	int iGraphicCounter;
	bool bSoundOn;
};

#endif // PIXEL_SPRITE_H
