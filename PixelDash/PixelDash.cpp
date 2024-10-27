
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
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
	std::wstring sLevel;
	int nFrameCounter = 0;
	int nLevelWidth;
	int nLevelHeight;
	bool bPlayerOnGround = false;

	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	std::unordered_set<wchar_t> moveAbleTiles = { L'.' };
	olc::Sprite* spriteTiles = nullptr;

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;

		
		//sLevel += L"................................................................";
		//sLevel += L"................................................................";
		//sLevel += L".......#####....................................................";
		//sLevel += L"........###.....................................................";
		//sLevel += L".......................########.................................";
		//sLevel += L".....##########.......###..............#.#......................";
		//sLevel += L"....................###................#.#......................";
		//sLevel += L"...................####.........................................";
		//sLevel += L"####################################.##############.....########";
		//sLevel += L"...................................#.#...............###........";
		//sLevel += L"........................############.#............###...........";
		//sLevel += L"........................#............#.........###..............";
		//sLevel += L"........................#.############......###.................";
		//sLevel += L"........................#................###....................";
		//sLevel += L"........................#################.......................";
		//sLevel += L"................................................................";
		
		
		
		sLevel += L"]______________________________________________________________[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"]..............................................................[";
		sLevel += L"X##############################################################X";
		sLevel += L"................................................................";

		spriteTiles = new olc::Sprite("assets/Terrain32x32.png");
		
		fPlayerPosX = 5;
		fPlayerPosY = 12;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		nFrameCounter++;
		auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				return sLevel[y * nLevelWidth + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t c)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				sLevel[y * nLevelWidth + x] = c;
		};


		if (GetKey(olc::UP).bHeld)
		{
			fPlayerVelY = -6.0f;
		}
		if (GetKey(olc::DOWN).bHeld)
		{
			fPlayerVelY = 6.0f;
		}
		if (GetKey(olc::RIGHT).bHeld)
		{
			fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
		}
		if (GetKey(olc::LEFT).bHeld)
		{
			fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
		}
		if (GetKey(olc::SPACE).bPressed)
		{
			if (fPlayerVelY == 0)
			{
				fPlayerVelY = -12.0f;
				//nDirModX = 1;
			}
		}

		// Gravity
		fPlayerVelY += 20.0f * fElapsedTime;

		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;


		// Drag
		if (bPlayerOnGround)
		{
			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < 0.01f)
				fPlayerVelX = 0.0f;
		}

		// Clamp velocities
		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;

		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;


		if (fPlayerVelX <= 0) // Moving Left
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f)) == 0)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				fPlayerVelX = 0;
			}
		}
		else // Moving Right
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f)) == 0)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;

			}
		}

		bPlayerOnGround = true;

		if (fPlayerVelY <= 0) // Moving Up
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY)) == 0)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else // Moving Down
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f)) == 0)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				//bPlayerOnGround = true; // Player has a solid surface underfoot
				//nDirModX = 0;
			}
		}

		// Apply new position
		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;

		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

		int nTileWidth = 32;
		int nTileHeight = 32;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;

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

		// Draw Level with camera offset
		for (int x = -1; x < nVisibleTilesX + 1; ++x)
		{
			for (int y = -1; y < nVisibleTilesY + 1; ++y)
			{
				wchar_t sTileID = GetTile(x + (int)fOffsetX, y + (int)fOffsetY);
				if (sTileID == L'#')
				{
					//FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::RED);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 64, 32, 32, 32, 1, 0);
				}
				else if (sTileID == L'.')
				{
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 64, 64, nTileWidth, nTileHeight, 1, 0);
				}
				else if (sTileID == L']')
				{
					DrawPartialSprite(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, spriteTiles, 96, 64, nTileWidth, nTileHeight, 1, 0);
				}
				else if (sTileID == L'[')
				{
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 32, 64, nTileWidth, nTileHeight, 1, 0);
				}
				else if (sTileID == L'_')
				{
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5 * 32, 5 * 32, nTileWidth, nTileHeight, 1, 0);
				}
				else if (sTileID == L'X')
				{
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 5*32, 5*32, nTileWidth, nTileHeight, 1, 0);
				}
			}
		}

		// Draw Player with respect to camera position
		FillRect((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, nTileWidth, nTileHeight, olc::GREEN);
		
		if (nFrameCounter % 50 == 0) {
			std::cout << "===================" << std::endl;
			std::cout << "nFrameCounter: " << nFrameCounter << std::endl;
			std::cout << std::endl;
			std::cout << "ScreenWidth: " << ScreenWidth() << std::endl;
			std::cout << "nVisibleTilesX: " << nVisibleTilesX << std::endl;
			std::cout << std::endl;
			std::cout << "fPlayerPosX: " << fPlayerPosX << std::endl;
			std::cout << "fOffsetX: " << fOffsetX << std::endl;
			std::cout << "nTileWidth: " << nTileWidth << std::endl;
			std::cout << std::endl;
			std::cout << "Player Drawing PosX: " << (fPlayerPosX - fOffsetX) * nTileWidth << std::endl;
			//std::cout << "fPlayerPosY: " << fPlayerPosY << std::endl;
			std::cout << std::endl;
			//std::cout << "fOffsetY: " << fOffsetY << std::endl;
		}

		
		// Called once per frame, draws random colored pixels
		/*for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));*/
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