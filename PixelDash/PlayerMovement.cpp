#include "PlayerMovement.h"

void PlayerMovement::ManageSpaceKey(float fElapsedTime)
{
}

PlayerMovement::PlayerMovement(olc::PixelGameEngine& pge, Player& player) : pge(pge), player(player) {}

void PlayerMovement::Update(float fElapsedTime)
{
	if (!bEnable) {
		return;
	}

	if (pge.GetKey(olc::Key::W).bHeld || pge.GetKey(olc::Key::UP).bHeld)
	{
		if (player.getVelY() == 0)
		{
			player.setVelY(-8.0f);
		}
	}


	if (pge.GetKey(olc::Key::S).bHeld || pge.GetKey(olc::Key::DOWN).bHeld)
	{
		player.setVelY(6.0f);
	}

	if (pge.GetKey(olc::Key::A).bHeld || pge.GetKey(olc::Key::LEFT).bHeld)
	{
		player.setVelX(player.getVelX() + (player.getPlayerOnGround() ? -25.0f : -15.0f) * fElapsedTime);
		player.eFacingDirection = Player::LEFT;
	}


	if (pge.GetKey(olc::Key::D).bHeld || pge.GetKey(olc::Key::RIGHT).bHeld)
	{
		player.setVelX(player.getVelX() + (player.getPlayerOnGround() ? 25.0f : 15.0f) * fElapsedTime);
		player.eFacingDirection = Player::RIGHT;
	}

	if (pge.GetKey(olc::Key::SPACE).bPressed && !player.getPlayerIsAttacking())
	{
		player.eGraphicState = Player::AnimationState::ATTACK;
		player.setPlayerIsAttacking(true);
		player.setGraphicCounter(0);  // Reset counter to start the attack animation from frame 0
		player.setGraphicTimer(0.0f); // Reset timer for consistent animation speed
	}

	if (pge.GetKey(olc::Key::E).bPressed)
	{
		player.setVelY(-6.0f);
	}

	if (pge.GetKey(olc::Key::Q).bPressed)
	{
		// lets quit
	}
}
