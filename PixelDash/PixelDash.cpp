
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "Level.h"
#include <memory>
#include <string>
#include <unordered_set>

// Override base class with your custom functionality
class PixelDash : public olc::PixelGameEngine
{
public:
	PixelDash()
	{
		sAppName = "PixelDash";
	}

private:
	/*std::wstring sLevel;
	std::wstring sDecoration;*/
	int nLevelWidth;
	int nLevelHeight;
	int nTileWidth;
	int nTileHeight;
	int nVisibleTilesX;
	int nVisibleTilesY;

	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	std::unique_ptr<Player> player;
	std::unique_ptr<PlayerMovement> pm;
	std::unique_ptr<Level> lb;
	


public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;	
		nTileWidth = 32;
		nTileHeight = 32;
		nVisibleTilesX = ScreenWidth() / nTileWidth;
		nVisibleTilesY = ScreenHeight() / nTileHeight;

		lb = std::make_unique<Level>(*this, nLevelWidth, nLevelHeight, nTileWidth, nTileHeight);
		player = std::make_unique<Player>(*this, *lb);
		pm = std::make_unique<PlayerMovement>(*this, *player);
		lb->Build();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		pm->Update(fElapsedTime);

		// Gravity
		player->setVelY(player->getVelY() + 20.0f * fElapsedTime);

		float fNewPlayerPosX = player->getPosX() + player->getVelX() * fElapsedTime;
		float fNewPlayerPosY = player->getPosY() + player->getVelY() * fElapsedTime;


		// Drag
		if (player->getPlayerOnGround())
		{
			player->setVelX(player->getVelX() - 3.0f * player->getVelX() * fElapsedTime);
			if (fabs(player->getVelX()) < 0.01f)
				player->setVelX(0.0f);
		}

		// Clamp velocities
		if (player->getVelX() > 10.0f)
			player->setVelX(10.0f);

		if (player->getVelX() < -10.0f)
			player->setVelX(-10.0f);

		if (player->getVelY() > 100.0f)
			player->setVelY(100.0f);

		if (player->getVelY() < -100.0f)
			player->setVelY(-100.0f);


		if (player->getVelX() <= 0) // Moving Left
		{
			if (lb->isMoveable(fNewPlayerPosX + 0.0f, player->getPosY() + 0.0f) || lb->isMoveable(fNewPlayerPosX + 0.0f, player->getPosY() + 0.9f))
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				player->setVelX(0.0f);
			}
		}
		else // Moving Right
		{
			if (lb->isMoveable(fNewPlayerPosX + 1.0f, player->getPosY() + 0.0f) || lb->isMoveable(fNewPlayerPosX + 1.0f, player->getPosY() + 0.9f))
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				player->setVelX(0.0f);
			}
		}

		if (player->getVelY() <= 0) // Moving Up
		{
			if (lb->isMoveable(fNewPlayerPosX + 0.0f, fNewPlayerPosY) || lb->isMoveable(fNewPlayerPosX + 0.9f, fNewPlayerPosY))
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				player->setVelY(0.0f);
				player->setPlayerOnGround(false);
			}
		}
		else // Moving Down
		{
			if (lb->isMoveable(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) || lb->isMoveable(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f))
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				player->setVelY(0.0f);
				player->setPlayerOnGround(true);
			}
		}

		if (!player->getPlayerIsAttacking()) {
			if (!player->getPlayerOnGround()) {
				if (player->getVelY() < -0.5f) {
					player->eGraphicState = Player::AnimationState::JUMP;
				}
				else if (player->getVelY() > 0.5f) {
					player->eGraphicState = Player::AnimationState::FALL;
				}
			}
			else {
				if (std::abs(player->getVelX()) >= 0.5f) {
					player->eGraphicState = Player::AnimationState::RUN;
				}
				else {
					player->eGraphicState = Player::AnimationState::IDLE;
				}
			}
		}

		// Apply new position
		player->setPosX(fNewPlayerPosX);
		player->setPosY(fNewPlayerPosY);

		fCameraPosX = player->getPosX();
		fCameraPosY = player->getPosY();

		// Calculate Top-Leftmost visible tile
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;

		// Clamp camera to game boundaries
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = nLevelWidth - nVisibleTilesX;
		if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = nLevelHeight - nVisibleTilesY;

		// Get tile offsets for smooth scrolling
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;
		
		lb->Update(fElapsedTime);
		lb->Draw(nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY, fTileOffsetX, fTileOffsetY);
		player->Update(fElapsedTime);
		player->setOffsets(fOffsetX, fOffsetY);
		player->Draw();

		return true;
	}
};

int main()
{
	PixelDash pd;
	if (pd.Construct(640, 480, 2, 2))
		pd.Start();
	return 0;
}
