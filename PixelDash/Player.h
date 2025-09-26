#ifndef PLAYER_H
#define PLAYER_H
#include "olcPixelGameEngine.h"
#include "Level.h"
#include "Box.h"
#include "Coin.h"
#include "TimedSprite.h"

#pragma once
class Player 
{
public:
	Player(olc::PixelGameEngine& pge, Level &lvl);
	
	void Update(float);
	void Draw();
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

	bool getPlayerIsAttacking() { return bIsAttacking; }
	void setPlayerIsAttacking(bool newBool) { bIsAttacking = newBool; }

	bool getForceAnimation() { return bForceAnimation; }
	void setForceAnimation(bool newBool) { bForceAnimation = newBool;}
	void startSound();
	float getGraphicTimer() { return fGraphicTimer; }
	void setGraphicTimer(float newTimer) { fGraphicTimer = newTimer; }
	void incGraphicCounter() { iGraphicCounter++; }
	void setGraphicCounter(float newCounter) { iGraphicCounter = (int)newCounter; }
	int getGraphicCounter() { return iGraphicCounter; }
	void setOffsets(float offsetX, float offsetY) { fOffsetX = offsetX; fOffsetY = offsetY; };
	float getOffsetX() { return fOffsetX; }
	float getOffsetY() { return fOffsetY; }
	const std::pair<float, float> getPlayerPos() { return std::pair<float, float>(fPlayerPosX, fPlayerPosY); };
	bool IsDoor();
	void openDoor();
	void attack();
	Rect GetAttackHitbox();
	Rect getPlayerRect();
	void hit();
	void heal();
	void earn();
	enum { RIGHT = 0, LEFT = 1} eFacingDirection;
	
	enum class AnimationState { IDLE, RUN, JUMP, FALL, ATTACK, DOOR_IN, DOOR_OUT, HIT};
	enum { ALIVE, DYING, DEAD } eLifeState;
	


private:
	olc::PixelGameEngine& pge;
	Level& lvl;
	olc::Sprite* spr;
	TimedSprite* bubble;
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
	bool bIsAttacking;
	bool bForceAnimation;
	float fAttackOffsetCorrection;
	int hitBoxWidth = 22;
	int hitBoxHeight = 30;
	int nOffsetCorrection = 0;
	float fOffsetX;
	float fOffsetY;
	float totalTime = 0.0f;
	int nScore = 0;
	float fLastHitTime = 0.0f;
	float fCooldownTime = 0.75f;
	
	void handleForceAnimation();
	void handleContinousAnimation();
	
	struct Animation {
		int iNumFrames;    // Number of frames in the animation
		int frameWidth;    // Width of each frame
		int frameHeight;   // Height of each frame
		float frameDuration; // Duration of each frame (for timing)
		int iOffsetPosY;
		int iSprOffsetX;
		int iSpecialOffsetY;
		int iSprOffsetY;
	};

	std::unordered_map<AnimationState, Animation> animations;
	Animation currentAnimation;

public:
	AnimationState eGraphicState; // Current animation state
	void setGraphicState(AnimationState state) { eGraphicState = state; };
};

#endif // PLAYER_H

