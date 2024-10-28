
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
	std::unordered_set<wchar_t> moveAbleTiles = { L'.', L'o', L'}', L'{', L'-', L',', L'v', L't', L'i', L'/', L'e' };
	std::unordered_map<wchar_t, std::pair<int, int>> tileOffsets;
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteDoor = nullptr;
	olc::Sprite* spritePlayer = nullptr;
	int iPlayerGraphicCounter = 0;
	float fGraphicTimer = 0.0f;
	int iFacing = 0;
	bool bIsRunning = false;

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;	
		
		
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"........<______________________________________________________>";
		sLevel += L"........]/----------------------------------------------------u[";
		sLevel += L"........]}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"........]}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"<_______i}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"]/-------eoooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"]}oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"]}oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"]v,,,,,,,toooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"l#######y}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		sLevel += L"........]v,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,z[";
		sLevel += L"........l######################################################r";
		sLevel += L"................................................................";
		sLevel += L"................................................................";

		spriteTiles = new olc::Sprite("assets/Terrain32x32.png");

		spriteDoor = new olc::Sprite("assets/IdleDoor.png");
		spritePlayer = new olc::Sprite("assets/King.png");
		
		tileOffsets = {
			{L'.', {64, 64}},
			{L'#', {64, 32}},
			{L']', {96, 64}},
			{L'[', {32, 64}},
			{L'y', {96, 32}},
			{L't', {256, 224}},
			{L'e', {256, 256}},
			{L'i', {96, 96}},
			{L'}', {32, 256}},
			{L'{', {96, 256}},
			{L'-', {64, 224}},
			{L',', {64, 288}},
			{L'/', {32, 224}},
			{L'u', {96, 224}},
			{L'v', {32, 288}},
			{L'z', {96, 288}},
			{L'l', {224, 64}},
			{L'r', {256, 64}},
			{L'<', {224, 32}},
			{L'>', {256, 32}},
			{L'_', {64, 96}},
			{L'P', {0, 0}},
			{L'o', {64, 256}}
		};

		fPlayerPosX = 5;
		fPlayerPosY = 8;

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
			iFacing = 0;
		}
		if (GetKey(olc::LEFT).bHeld)
		{
			fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
			iFacing = 1;
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
		
		bIsRunning = !(std::abs(fPlayerVelX) >= 0.5f);

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
				int sx = tileOffsets[sTileID].first;
				int sy = tileOffsets[sTileID].second;
				DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, sx, sy, nTileWidth, nTileHeight, 1, 0);

			}
		}

		// Draw Player with respect to camera position
		//FillRect((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, nTileWidth, nTileHeight, olc::GREEN);
		fGraphicTimer += fElapsedTime;
		
		int iNumFrames = 0;
		int oy = 0;
		if (bIsRunning) {
			iNumFrames = 11;
			oy = 0;
		}
		else {
			iNumFrames = 8;
			oy = 28;
		}
		if (fGraphicTimer > 0.1f) {
			fGraphicTimer -= 0.1f;
			iPlayerGraphicCounter++;
			iPlayerGraphicCounter %= iNumFrames;
		}
		
		SetPixelMode(olc::Pixel::MASK);
		DrawPartialSprite((fPlayerPosX - fOffsetX) * nTileWidth, ((fPlayerPosY - fOffsetY) * nTileHeight) + 4, spritePlayer, 37*iPlayerGraphicCounter, oy, 37, 28, 1, iFacing);
		SetPixelMode(olc::Pixel::NORMAL);

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