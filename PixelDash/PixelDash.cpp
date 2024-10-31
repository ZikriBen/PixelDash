
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Player.h"
#include "PlayerMovement.h"
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
	std::wstring sLevel;
	std::wstring sDecoration;
	int nLevelWidth;
	int nLevelHeight;
	int nTileWidth;
	int nTileHeight;
	int nVisibleTilesX;
	int nVisibleTilesY;

	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	std::unique_ptr<PlayerMovement> pm;
	std::unordered_set<wchar_t> moveAbleTiles = { L'.', L'o', L'}', L'{', L'-', L',', L'v', L't', L'i', L'/', L'e', L'u', L'z' };
	std::unordered_map<wchar_t, std::pair<int, int>> tileOffsets;
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteDoor = nullptr;
	std::unique_ptr<Player> player;
	std::unordered_map<wchar_t, std::vector<std::pair<int, int>>> decorPos;



public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;	
		nTileWidth = 32;
		nTileHeight = 32;
		nVisibleTilesX = ScreenWidth() / nTileWidth;
		nVisibleTilesY = ScreenHeight() / nTileHeight;

		player = std::make_unique<Player>(*this);
		pm = std::make_unique<PlayerMovement>(*this, *player);

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

		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"...........................D....................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";
		sDecoration += L"................................................................";

		// create decoration array
		for (int x = 0; x < nLevelWidth; ++x)
		{
			for (int y = 0; y < nLevelHeight; ++y)
			{
				wchar_t cDecorID = sDecoration[y * nLevelWidth + x];
				if (cDecorID != '.') {
					decorPos[cDecorID].push_back({ x, y });
				}
			}
		}

		spriteTiles = new olc::Sprite("assets/Terrain32x32.png");

		spriteDoor = new olc::Sprite("assets/IdleDoor.png");
		

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

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
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
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, player->getPosY() + 0.0f)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, player->getPosY() + 0.9f)) == 0)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				player->setVelX(0.0f);
			}
		}
		else // Moving Right
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 1.0f, player->getPosY() + 0.0f)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 1.0f, player->getPosY() + 0.9f)) == 0)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				player->setVelX(0.0f);
			}
		}

		

		if (player->getVelY() <= 0) // Moving Up
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY)) == 0)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				player->setVelY(0.0f);
				player->setPlayerOnGround(false);
			}
		}
		else // Moving Down
		{
			if (moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)) == 0 || moveAbleTiles.count(GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f)) == 0)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				player->setVelY(0.0f);
				player->setPlayerOnGround(true);
			}
		}


		// Apply new position
		player->setPosX(fNewPlayerPosX);
		player->setPosY(fNewPlayerPosY);

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

		int startX = static_cast<int>(fOffsetX);  // Leftmost visible tile in the level
		int startY = static_cast<int>(fOffsetY);  // Topmost visible tile in the level

		for (const auto& entry : decorPos) {
			wchar_t decorID = entry.first;

			for (const auto& pos : entry.second) {
				int levelX = pos.first; 
				int levelY = pos.second; 

				if (levelX >= startX && levelX < startX + nVisibleTilesX +1 &&
					levelY >= startY && levelY < startY + nVisibleTilesY) {

					int screenX = (levelX - startX) * nTileWidth - fTileOffsetX;
					int screenY = (levelY - startY) * nTileHeight - fTileOffsetY - 24;
					SetPixelMode(olc::Pixel::MASK);
					DrawSprite(screenX, screenY, spriteDoor, 1, 0);
					SetPixelMode(olc::Pixel::NORMAL);
				}
			}
		}

		player->Update(fElapsedTime);
		
		player->Draw(fOffsetX, fOffsetY);

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