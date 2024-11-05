#include "Level.h"
#include "Box.h"
#include "Coin.h"

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
	sDecoration += L".....OOOO.......................................................";
	sDecoration += L"................................................................";
	sDecoration += L".......................................OOOO.....................";
	sDecoration += L"...........................D...........OOOO.....................";
	sDecoration += L"...................................B............................";
	sDecoration += L"................................................................";
	sDecoration += L"................................................................";

	// create decoration array
	for (int x = 0; x < nLevelWidth; ++x)
	{
		for (int y = 0; y < nLevelHeight; ++y)
		{
			wchar_t cDecorID = sDecoration[y * nLevelWidth + x];
			if (cDecorID == 'D') {
				sDoorOpen = new PixelSprite(pge, "assets/DoorOpening.png", 4, 0.2, 46, 56, 0, 0);
				pixelSprites[cDecorID].emplace_back(
					sDoorOpen,
					std::make_pair(x, y)
				);
			}
			else if (cDecorID == 'B') {
				pixelSprites[cDecorID].emplace_back(
					new Box(pge, "assets/Box.png", 0, 0, 1, 0.2, 22, 16, 0, 0, 0, 8),
					std::make_pair(x, y)
				);
				
			}
			else if (cDecorID == 'O') {
				Coin* c = new Coin(pge, "assets/BigDiamond.png", 0, 0, 9, 0.1, 18, 14, 2, 0, 0, 8);
				c->setAnimation(true);
				c->setLoop(true);
				pixelSprites[cDecorID].emplace_back(
					c,
					std::make_pair(x, y)
				);
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
	for (const auto& entry : pixelSprites) {
		for (const auto& spritePos : entry.second) {
			PixelSprite* sprite = spritePos.first;
			if (spritePos.first) {
				spritePos.first->Update(fElapsedTime);
			}
		}
	}
	//sDoorOpen->Update(fElapsedTime);
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

	for (const auto& entry : pixelSprites) {
		for (const auto& spritePos : entry.second) {
			PixelSprite* sprite = spritePos.first;
			auto [levelX, levelY] = spritePos.second;

			if (sprite) {
				if (levelX >= startX && levelX < startX + nVisibleTilesX + 1 &&
					levelY >= startY && levelY < startY + nVisibleTilesY) {
					int screenX = (levelX - startX) * nTileWidth - fTileOffsetX;
					int screenY = (levelY - startY) * nTileHeight - fTileOffsetY - 24;
					sprite->setPosX(screenX);
					sprite->setPosY(screenY);
					sprite->Draw();
				}
			}
		}
	}
}

bool Level::isMoveable(int x, int y)
{
	return moveAbleTiles.count(GetTile(x, y)) == 0;
}

bool Level::isDoor(float x, float y) {
	auto it = pixelSprites.find('D');
	if (it == pixelSprites.end()) {
		return false; 
	}

	// Iterate through each door's position in the vector
	for (const auto& entry : it->second) {
		PixelSprite* sprite = entry.first; // Get the PixelSprite pointer (door sprite)
		auto [doorX, doorY] = entry.second; // Get the door position

		// Check if the player is within 0.5 units of the door position
		if (std::abs(x - doorX) < 0.5 && std::abs(y - doorY) < 0.5) {
			return true; // Player is at the door
		}
	}

	return false; 
}

void Level::openDoor() {
	sDoorOpen->setAnimation(true);
}

PixelSprite*Level::checkCollisionWithDecorations(const Rect& playerRect) {
	// Loop through the pixelSprites decoration map
	for (const auto& entry : pixelSprites) {
		// For each decoration type, we might have multiple positions
		for (const auto& spritePos : entry.second) {
			PixelSprite* sprite = spritePos.first;
			auto [levelX, levelY] = spritePos.second;

			// If it's a Box or other interactive object, we can check for collisions

			Rect hitbox = sprite->getRect();

			// Check if player and box hitboxes intersect
			if (playerRect.intersects(hitbox)) {
				return sprite;
			}
		}
	}
	return nullptr;
}

void Level::removeDecoration(PixelSprite* decoration) {
	for (auto& entry : pixelSprites) {
		auto& vec = entry.second;
		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[decoration](const std::pair<PixelSprite*, std::pair<float, float>>& item) {
				return item.first == decoration;
			}), vec.end());
	}
	delete decoration;  // Free memory if the decoration was dynamically allocated
}