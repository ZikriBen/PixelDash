#include "Player.h"

Player::Player(olc::PixelGameEngine& pge, Level& lvl) : pge(pge), lvl(lvl), eLifeState(Player::ALIVE), bSoundOn(true) {
	spr = new olc::Sprite("assets/IdleRun.png");
	fWidth = 37.0f;
	fHeight = 28.0f;
	fGraphicTimer = 0.0f;
	iGraphicCounter = 0;
	
	fPlayerPosX = 5;
	fPlayerPosY = 8;

	fPlayerVelX = 0.0f;
	fPlayerVelY = 0.0f;
	eGraphicState = AnimationState::IDLE;
	bPlayerOnGround = false;
	bIsAttacking = false;
	bForceAnimation = false;
	eFacingDirection = Player::RIGHT;
	animations = {
		{AnimationState::IDLE, {11, 37, 28, 0.1f, 4, 41, 0, 16}},
		{AnimationState::RUN, {8, 37, 28, 0.1f, 4, 41, 58, 16}},
		{AnimationState::FALL, {1, 37, 28, 0.1f, 4, 41, 116, 16}},  
		{AnimationState::JUMP, {1, 37, 28, 0.1f, 4, 41, 174, 16}},
		{AnimationState::ATTACK, {3, 59, 58, 0.1f, -11, 12, 232, 0}},
		{AnimationState::DOOR_IN,  {8, 39, 42, 0.1f, -11, 41, 290, 0}},
		{AnimationState::DOOR_OUT,  {8, 41, 42, 0.1f, -11, 35, 348, 0}}
	};
}

void Player::Update(float fElapsedTime) {
	setGraphicTimer(getGraphicTimer() + fElapsedTime);
	currentAnimation = animations[eGraphicState];

	if (bForceAnimation) {

		// Play attack animation until the last frame, then reset
		if (getGraphicTimer() > currentAnimation.frameDuration) {
			setGraphicTimer(getGraphicTimer() - currentAnimation.frameDuration);
			incGraphicCounter();

			if (getGraphicCounter() >= currentAnimation.iNumFrames) {
				// Attack animation is complete, reset counter and switch back to idle/run
				setGraphicCounter(0); // Reset the counter
				eGraphicState = (std::abs(getVelX()) >= 0.5f) ? AnimationState::RUN : AnimationState::IDLE;
				currentAnimation = animations[eGraphicState]; // Force update to new animation
				
				if (bIsAttacking)
					bIsAttacking = false;
			}
		}
	}
	else {
		// Regular state logic for idle, run, jump, etc.
		if (currentAnimation.iNumFrames > 1 && getGraphicTimer() > currentAnimation.frameDuration) {
			setGraphicTimer(getGraphicTimer() - currentAnimation.frameDuration);
			incGraphicCounter();
			setGraphicCounter(getGraphicCounter() % currentAnimation.iNumFrames); // Wrap within valid frame range
		}
		else if (currentAnimation.iNumFrames == 1) {
			setGraphicCounter(0);
		}
	}
}

void Player::Draw(float fOffsetX, float fOffsetY) {
	
	pge.SetPixelMode(olc::Pixel::MASK);
	pge.DrawPartialSprite(
		(fPlayerPosX - fOffsetX) * 32,
		((fPlayerPosY - fOffsetY) * 32) + currentAnimation.iOffsetPosY, // +4
		spr,
		(currentAnimation.iSprOffsetX + currentAnimation.frameWidth) * (float)(iGraphicCounter), // 41 + 37
		currentAnimation.iSpecialOffsetY + currentAnimation.iSprOffsetY, // 16
		currentAnimation.frameWidth, // 37
		currentAnimation.frameHeight, //28
		1,
		eFacingDirection
	);
	pge.SetPixelMode(olc::Pixel::NORMAL);
}

bool Player::IsDoor() {
	return lvl.isDoor(fPlayerPosX, fPlayerPosY);
}