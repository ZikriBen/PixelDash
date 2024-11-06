#include "PlayerMovement.h"


PlayerMovement::PlayerMovement(olc::PixelGameEngine& pge, Player& player) : pge(pge), player(player) {}

void PlayerMovement::Update(float fElapsedTime)
{
	if (!bEnable) {
		return;
	}

	if (pge.GetKey(olc::Key::W).bHeld || pge.GetKey(olc::Key::UP).bHeld)
	{
		if (player.IsDoor()) {
			player.openDoor();
		}
		else {
			if (player.getVelY() == 0)
			{
				player.setVelY(-9.5f);
			}
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
		player.attack();
	}

	if (pge.GetKey(olc::Key::E).bPressed)
	{
		player.setVelY(-6.0f);
	}

	if (pge.GetKey(olc::Key::Q).bPressed)
	{
		player.hit();
	}
	
	if (pge.GetKey(olc::Key::R).bPressed)
	{
		player.heal();
	}

	if (pge.GetKey(olc::Key::T).bPressed)
	{
		player.earn();
	}
}
