
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>

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
	
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;
	
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;

		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L".......#####....................................................";
		sLevel += L"........###.....................................................";
		sLevel += L".......................########.................................";
		sLevel += L".....##########.......###..............#.#......................";
		sLevel += L"....................###................#.#......................";
		sLevel += L"...................####.........................................";
		sLevel += L"####################################.##############.....########";
		sLevel += L"...................................#.#...............###........";
		sLevel += L"........................############.#............###...........";
		sLevel += L"........................#............#.........###..............";
		sLevel += L"........................#.############......###.................";
		sLevel += L"........................#................###....................";
		sLevel += L"........................#################.......................";
		sLevel += L"................................................................";

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



		// Handle Input
		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;


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
			fPlayerVelX = 6.0f;
		}
		if (GetKey(olc::LEFT).bHeld)
		{
			fPlayerVelX = -6.0f;
		}

		fPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		fPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

		int nTileWidth = 8;
		int nTileHeight = 8;
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
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::RED);
				}
				else if (sTileID == L'.')
				{
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::CYAN);
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
	if (pd.Construct(160, 120, 8, 8))
		pd.Start();
	return 0;
}