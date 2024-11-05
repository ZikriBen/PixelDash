#include "Player.h"
#include "Box.h"
#include "Coin.h"

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

	fAttackOffsetCorrection = animations[AnimationState::ATTACK].frameWidth - fWidth;
}

void Player::Update(float fElapsedTime) {
	totalTime += fElapsedTime;
	setGraphicTimer(getGraphicTimer() + fElapsedTime);
	currentAnimation = animations[eGraphicState];

	if (PixelSprite* ps = lvl.checkCollisionWithDecorations(getPlayerRect())) {
		if (Coin* c = dynamic_cast<Coin*>(ps)) {
			if (!c->isCollected()) {
				c->collect();
				lvl.removeDecoration(c);
			}
		}
	}

	if (bIsAttacking) {

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

		if (PixelSprite* ps = lvl.checkCollisionWithDecorations(GetAttackHitbox())) {
			if (Box* b = dynamic_cast<Box*>(ps)) {
				b->hit(totalTime);
			}
		}
	}
	else {
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

void Player::Draw() {
	nOffsetCorrection = 0;
	float hitboxX = ((fPlayerPosX - fOffsetX) * 32) + fWidth;;

	if (eFacingDirection == LEFT) {
		nOffsetCorrection = currentAnimation.frameWidth - fWidth;
		hitboxX = ((fPlayerPosX - fOffsetX) * 32) - nOffsetCorrection;
	}

	pge.SetPixelMode(olc::Pixel::MASK);

	pge.DrawPartialSprite(
		((fPlayerPosX - fOffsetX) * 32) - nOffsetCorrection,
		((fPlayerPosY - fOffsetY) * 32) + currentAnimation.iOffsetPosY,
		spr,
		((currentAnimation.iSprOffsetX + currentAnimation.frameWidth) * (float)(iGraphicCounter)), 
		currentAnimation.iSpecialOffsetY + currentAnimation.iSprOffsetY,
		currentAnimation.frameWidth,
		currentAnimation.frameHeight,
		1,
		eFacingDirection
	);
	pge.SetPixelMode(olc::Pixel::NORMAL);
	
	pge.DrawRect(getPlayerRect().x, getPlayerRect().y, currentAnimation.frameWidth, currentAnimation.frameHeight);
	pge.DrawRect(GetAttackHitbox().x, GetAttackHitbox().y, hitBoxWidth, hitBoxHeight, olc::RED);
}

bool Player::IsDoor() {
	return lvl.isDoor(fPlayerPosX, fPlayerPosY);
}

void Player::openDoor()
{
	lvl.openDoor();
}

Rect Player::getPlayerRect() {
	return Rect(
		((fPlayerPosX - fOffsetX) * 32) - nOffsetCorrection,
		((fPlayerPosY - fOffsetY) * 32) + currentAnimation.iOffsetPosY,
		(float)currentAnimation.frameWidth,
		(float)currentAnimation.frameHeight
	);
}

Rect Player::GetAttackHitbox() {
	nOffsetCorrection = 0;
	float hitboxX = ((fPlayerPosX - fOffsetX) * 32) + fWidth;;

	if (eFacingDirection == LEFT) {
		nOffsetCorrection = currentAnimation.frameWidth - fWidth;
		hitboxX = ((fPlayerPosX - fOffsetX) * 32) - nOffsetCorrection;
	}
	
	return Rect(hitboxX, (fPlayerPosY - fOffsetY) * 32, hitBoxWidth, hitBoxHeight);
}