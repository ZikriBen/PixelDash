#ifndef PLAYER_H
#define PLAYER_H
#include "olcPixelGameEngine.h"

#pragma once
class Player
{
public:
	Player(olc::PixelGameEngine& pge);
	
	void Update(float);
	void Draw(float fOffsetX, float fOffsetY, float oy);
	float getWidth() { return fWidth; };
	float getHeight() { return fHeight; };
	float getCurrentHealth() { return fCurrentHealth; }
	void setVelX(float newVel) { fPlayerVelX = newVel; };
	void setVelY(float newVel) { fPlayerVelY = newVel; };
	float getVelX() { return fPlayerVelX; };
	float getVelY() { return fPlayerVelY; };

	void setPosX(float newPos) { fPlayerPosX = newPos; };
	void setPosY(float newPos) { fPlayerPosY = newPos; };
	float getPosX() { return fPlayerPosX; };
	float getPosY() { return fPlayerPosY; };

	bool getPlayerOnGround() { return bPlayerOnGround; }
	void setPlayerOnGround(bool newBool) { bPlayerOnGround = newBool; }

	float getGraphicTimer() { return fGraphicTimer; }
	void setGraphicTimer(float newTimer) { fGraphicTimer = newTimer; }
	void incGraphicCounter() { iGraphicCounter++; }
	void setGraphicCounter(float newCounter) { iGraphicCounter = newCounter; }
	int getGraphicCounter() { return iGraphicCounter; }

	enum { RIGHT = 0, LEFT = 1} eFacingDirection;
	enum { IDLE, RUN, JUMP, FALL, ATTACK, HIT, DOOR_IN, DOOR_OUT} eGraphicState;
	enum { ALIVE, DYING, DEAD } eLifeState;


private:
	olc::PixelGameEngine& pge;
	olc::Sprite* spr;
	bool bSoundOn;
	float fWidth;
	float fHeight;
	float fCurrentHealth;
	float fGraphicTimer;
	int iGraphicCounter;
	float fPlayerPosX;
	float fPlayerPosY;
	float fPlayerVelX;
	float fPlayerVelY;
	bool bPlayerOnGround;
};

#endif // PLAYER_H

