#include "Level.h"

Level::Level(olc::PixelGameEngine& pge, int levelWidth, int levelHeight, int tileWidth, int tileHeight) : 
	pge(pge), 
	nTileWidth(tileWidth), 
	nTileHeight(tileHeight),
	nLevelWidth(levelWidth), 
	nLevelHeight(levelHeight) 
{}


void Level::Build()
{
	moveAbleTiles = { L'.', L'o', L'}', L'{', L'-', L',', L'v', L't', L'/', L'e', L'u', L'z' };
	spriteTiles = new olc::Sprite("assets/Terrain32x32.png");
	spriteDoor = new olc::Sprite("assets/IdleDoor.png");
	sDoorOpen = new PixelSprite(pge, "assets/DoorOpening.png", 4, 0.2, 46, 56, 0, 0);

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
}

wchar_t Level::GetTile(int x, int y)
{
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
		return sLevel[y * nLevelWidth + x];
	else
		return L' ';
}

void Level::SetTile(int x, int y, wchar_t c)
{
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
		sLevel[y * nLevelWidth + x] = c;
}

void Level::Update(float fElapsedTime)
{
	sDoorOpen->Update(fElapsedTime);
}

void Level::Draw(int nVisibleTilesX, int nVisibleTilesY, float fOffsetX, float fOffsetY, float fTileOffsetX, float fTileOffsetY)
{
	// Draw Level with camera offset
	for (int x = -1; x < nVisibleTilesX + 1; ++x)
	{
		for (int y = -1; y < nVisibleTilesY + 1; ++y)
		{
			wchar_t sTileID = GetTile(x + (int)fOffsetX, y + (int)fOffsetY);

			int sx = tileOffsets[sTileID].first;
			int sy = tileOffsets[sTileID].second;
			pge.DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, sx, sy, nTileWidth, nTileHeight, 1, 0);
		}
	}

	int startX = static_cast<int>(fOffsetX);  // Leftmost visible tile in the level
	int startY = static_cast<int>(fOffsetY);  // Topmost visible tile in the level

	for (const auto& entry : decorPos) {
		wchar_t decorID = entry.first;

		for (const auto& pos : entry.second) {
			int levelX = pos.first;
			int levelY = pos.second;

			if (levelX >= startX && levelX < startX + nVisibleTilesX + 1 &&
				levelY >= startY && levelY < startY + nVisibleTilesY) {

				int screenX = (levelX - startX) * nTileWidth - fTileOffsetX;
				int screenY = (levelY - startY) * nTileHeight - fTileOffsetY - 24;
				sDoorOpen->setPosX(screenX);
				sDoorOpen->setPosY(screenY);
				sDoorOpen->Draw();
				//pge.SetPixelMode(olc::Pixel::MASK);
				//pge.DrawSprite(screenX, screenY, spriteDoor, 1, 0);
				//pge.SetPixelMode(olc::Pixel::NORMAL);
			}
		}
	}
}

bool Level::isMoveable(int x, int y)
{
	return moveAbleTiles.count(GetTile(x, y)) == 0;
}

bool Level::isDoor(float x, float y) {
	for (const auto& decor : decorPos) 
		if (decor.first == 'D') 
			for (const auto& door : decor.second) 
			{
				if(std::abs(x - door.first) < 0.5 && std::abs(y - door.second) < 0.5)
					return true;
			}

	return false;
}

void Level::openDoor() {
	sDoorOpen->setAnimation(true);
}
			
