
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


public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;	
		nTileWidth = 32;
		nTileHeight = 32;
		nVisibleTilesX = ScreenWidth() / nTileWidth;
		nVisibleTilesY = ScreenHeight() / nTileHeight;

		Level::Init(*this, nLevelWidth, nLevelHeight, nTileWidth, nTileHeight);
		Level& lb = Level::getInstance();

		player = std::make_unique<Player>(*this, lb);
		pm = std::make_unique<PlayerMovement>(*this, *player);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Level& lb = Level::getInstance();
		pm->Update(fElapsedTime);

		// Gravity
		player->setVelY(player->getVelY() + 18.5f * fElapsedTime);
		
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

		float fNewPlayerPosX = player->getPosX() + player->getVelX() * fElapsedTime;
		float fNewPlayerPosY = player->getPosY() + player->getVelY() * fElapsedTime;
		
		
		
		if (player->getVelX() <= 0) // Moving Left
		{
			if (lb.isMoveable(fNewPlayerPosX + 0.0f, player->getPosY() + 0.0f) || lb.isMoveable(fNewPlayerPosX + 0.0f, player->getPosY() + 0.9f))
			{
				fNewPlayerPosX = std::trunc(fNewPlayerPosX) + 1.0f;
				player->setVelX(0.0f);
			}
		}
		else // Moving Right
		{
			if (lb.isMoveable(fNewPlayerPosX + 1.0f, player->getPosY() + 0.0f) || lb.isMoveable(fNewPlayerPosX + 1.0f, player->getPosY() + 0.9f))
			{
				fNewPlayerPosX = std::trunc(fNewPlayerPosX);
				player->setVelX(0.0f);
			}
		}

		if (player->getVelY() < 0) // Moving Up
		{
			player->setPlayerOnGround(false);
			if (lb.isMoveable(fNewPlayerPosX + 0.0f, fNewPlayerPosY) ||
				lb.isMoveable(fNewPlayerPosX + 0.9f, fNewPlayerPosY))
			{
				
				fNewPlayerPosY = std::trunc(fNewPlayerPosY) + 1.0f;
				player->setVelY(0.0f);
				
			}
		}
		else if (player->getVelY() > 0) // Moving Down
		{
			if (lb.isPlatform(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) || // Check one tile above
				lb.isPlatform(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f))
			{
				if (PixelSprite* ps = lb.checkCollisionWithDecorations(player->getPlayerRect()))
					if (Platform* p = dynamic_cast<Platform*>(ps)) {

						if ((fNewPlayerPosY) < p->getHomeY() - 0.5) {
							fNewPlayerPosY = std::trunc(fNewPlayerPosY);
							player->setVelY(0.0f);
							player->setPlayerOnGround(true);
						}
					}
				
			}
			if (lb.isMoveable(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) ||
				lb.isMoveable(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f))
			{
				fNewPlayerPosY = std::trunc(fNewPlayerPosY);
				player->setVelY(0.0f);
				player->setPlayerOnGround(true);
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
		if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = (float)(nLevelWidth - nVisibleTilesX);
		if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = (float)(nLevelHeight - nVisibleTilesY);

		// Get tile offsets for smooth scrolling
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;
		
		lb.Update(fElapsedTime);
		lb.Draw(nVisibleTilesX, nVisibleTilesY, fOffsetX, fOffsetY, fTileOffsetX, fTileOffsetY);
		player->setOffsets(fOffsetX, fOffsetY);
		player->Update(fElapsedTime);
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
