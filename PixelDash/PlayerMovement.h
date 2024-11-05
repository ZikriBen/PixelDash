#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H
#include "Player.h"
#include "olcPixelGameEngine.h"

#pragma once
class Player;

class PlayerMovement
{

private:
	bool bEnable = true;

public:
	PlayerMovement(olc::PixelGameEngine& pge, Player& player);
	olc::PixelGameEngine& pge;
	Player& player;


	/// <summary>
	/// Call this method to update the player poistion
	/// </summary>
	/// <param name="fElapsedTime">fElapsedTime</param>
	void Update(float fElapsedTime);
	void setEnable(bool isEnable) { bEnable = isEnable; }
};
#endif // PLAYER_MOVEMENT_H