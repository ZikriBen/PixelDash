#include "Level.h"

Level* Level::instance = nullptr;

Level::Level(int levelWidth, int levelHeight, int tileWidth, int tileHeight)
	: nLevelWidth(levelWidth), nLevelHeight(levelHeight), nTileWidth(tileWidth), nTileHeight(tileHeight)
{
	// Empty constructor body; initialization happens in Init
}

void Level::Init(olc::PixelGameEngine& pge, int levelWidth, int levelHeight, int tileWidth, int tileHeight)
{
	if (!instance)
	{
		instance = new Level(levelWidth, levelHeight, tileWidth, tileHeight);
		instance->pge = &pge;  // Store the game engine reference
		instance->moveAbleTiles = { L'.', L'o', L'}', L'{', L'-', L',', L'v', L't', L'/', L'e', L'u', L'z', L' '};
		instance->spriteTiles = new olc::Sprite("assets/Terrain32x32.png");
		instance->spriteDoor = new olc::Sprite("assets/IdleDoor.png");

		HUD::init(pge);
		
		

		//instance->sLevel += L"................................................................";
		//instance->sLevel += L"................................................................";
		//instance->sLevel += L"........<______________________________________________________>";
		//instance->sLevel += L"........]/----------------------------------------------------u[";
		//instance->sLevel += L"........]}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"........]}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"<_______i}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"]/-------eoooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"]}oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"]}oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"]v,,,,,,,toooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"l#######y}oooooooooooooooooooooooooooooooooooooooooooooooooooo{[";
		//instance->sLevel += L"........]v,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,z[";
		//instance->sLevel += L"........l######################################################r";
		//instance->sLevel += L"................................................................";
		//instance->sLevel += L"................................................................";

		instance->sLevel +=      L".................<______>......<_______________________________>";
		instance->sLevel +=      L".................]}oooo{[......]/-----------------------------u[";
		instance->sLevel +=      L".................]}oooo{[......]}ooooooooooooooooooooooooooooo{[";
		instance->sLevel +=      L".................]}oooo{[......]}ooooooooooooooooooooooooooooo{[";
		instance->sLevel +=      L"......<__________i}oooo{[......]}ooooooooooooooooooooooooooooo{[";
		instance->sLevel +=      L"......]/----------ooooo{y______ioooooooooooooooooooooooooooxccc5";
		instance->sLevel +=      L"......]v,,,,,,,,oooooooo--------oooooooooooooooooooooooooooooo{[";
		instance->sLevel +=      L".<____4cccccc3cb}oooooooooooooooooooooooooooooooooooor##pooooo{[";
		instance->sLevel +=      L".]/---------um/-o,,,,,,,,,ooooooooooooooooooooooooooo[..]ooooo{[";
		instance->sLevel +=      L".]}ooooooooo{m}o{xc6###7cb}oooooooooooooooooooooooooo[..]ooooo{[";
		instance->sLevel +=      L".]}ooooooooo{k}oo-uy___i/-oooooooooooooooooooooooooors..]ooooo{[";
		instance->sLevel +=      L".]}oooooooooo-ooooo-----oooo,,ooooooooooooooooooor##s...]ooooo{[";
		instance->sLevel +=      L".]}ooooooooooooo,,,,,,,,oo,zrpvoooooooooooooooor#s......]ooooo{[";
		instance->sLevel +=      L".]}oooooooooooo{2ccccccb}{r#slp}ooooooooooooooo[........]ooooo{[";
		instance->sLevel +=      L".]v,,,,,,,,,,,,zm,,,,,,,,z[...]v,,,,,,,r#####p,[........],,,,,z[";
		instance->sLevel +=      L".l##############1#########s...l########s.....l#s........l######s";

		instance->sDecoration += L"................................................................";
		instance->sDecoration += L"................................................................";
		instance->sDecoration += L"...................ZC...........................................";
		instance->sDecoration += L"................................................................";
		instance->sDecoration += L"......................ZX.....................................D..";
		instance->sDecoration += L"................................................................";
		instance->sDecoration += L"..................ZC............................................";
		instance->sDecoration += L"...............................................V................";
		instance->sDecoration += L"....................................................V...........";
		instance->sDecoration += L"......OOO....................................................ZX.";
		instance->sDecoration += L"................................................................";
		instance->sDecoration += L".....V.........V.................HH....OOOO..............XC.....";
		instance->sDecoration += L".......................................OOOO.....................";
		instance->sDecoration += L".............................................................ZX.";
		instance->sDecoration += L"................................................................";
		instance->sDecoration += L"..........E.......H................B......................OOOOO.";


		// create decoration array
		for (int x = 0; x < instance->nLevelWidth; ++x)
		{
			for (int y = 0; y < instance->nLevelHeight; ++y)
			{
				wchar_t cDecorID = instance->sDecoration[y * instance->nLevelWidth + x];
				if (cDecorID == 'D') {
					instance->sDoorOpen = new PixelSprite(pge, "assets/DoorOpening.png", 4, 0.2, 46, 56, 0, 0);
					instance->pixelSprites[cDecorID].emplace_back(
						instance->sDoorOpen,
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'B') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Box(pge, "assets/Box.png", 0, 0, 1, 0.2, 22, 16, 0, 0, 0, 8),
						std::make_pair(x, y)
					);

				}
				else if (cDecorID == 'O') {
					Coin* c = new Coin(pge, "assets/BigDiamond.png", 0, 0, 9, 0.1, 18, 14, 2, 0, 0, 8);
					c->setAnimation(true);
					c->setLoop(true);
					instance->pixelSprites[cDecorID].emplace_back(
						c,
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'H') {
					Heart* h = new Heart(pge, "assets/BigHeart.png", 0, 0, 7, 0.1, 18, 14, 2, 0, 0, 8);
					h->setAnimation(true);
					h->setLoop(true);
					instance->pixelSprites[cDecorID].emplace_back(
						h,
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'E') {
					instance->enemy = new Enemy(pge, "assets/Pig.png", x, y, 10, 0.1, 34, 28, 2, 0, 0, 0);
					instance->enemy->setAnimation(true);
					instance->enemy->setLoop(true);
				}
				else if (cDecorID == 'W') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Window(pge, WindowType::ONE),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'Z') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::ONE, x, y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'X') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::TWO, x, y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'C') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::THREE, x, y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'V') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::FOUR, x, y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'F') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Flag(pge, FlagType::TWO),
						std::make_pair(x, y)
					);
				}
			}
		}

		instance->tileOffsets = {
				{L'r', {32, 32}},
				{L'[', {32, 64}},
				{L'y', {32, 96}},
				
				{L'x', {32, 160}},
				{L'c', {64, 160}},
				{L'b', {96, 160}},

				{L'n', {160, 32}},
				{L'm', {160, 64}},
				{L'k', {160, 96}},
				
				{L'/', {32, 224}},
				{L'}', {32, 256}},
				{L'v', {32, 288}},

				{L'#', {64, 32}},
				{L'.', {64, 64}},
				{L'_', {64, 96}},
				{L'-', {64, 224}},
				{L'o', {64, 256}},
				{L',', {64, 288}},

				{L'p', {96, 32}},
				{L']', {96, 64}},
				{L'i', {96, 96}},
				{L'u', {96, 224}},
				{L'z', {96, 288}},
				{L'{', {96, 256}},
				
				{L'<', {224, 32}},
				{L'l', {224, 64}},

				{L'>', {256, 32}},
				{L's', {256, 64}},
				{L't', {256, 224}},
				{L'e', {256, 256}},

				{L'1', {352, 32}},
				{L'2', {512, 128}},
				{L'3', {416, 128}},
				{L'4', {224, 160}},
				{L'5', {320, 32}},
				{L'6', {256, 128}},
				{L'7', {224, 128}},

				{L'P', {0, 0}},
				
		};
	}
}

Level& Level::getInstance()
{
	if (!instance)
	{
		throw std::runtime_error("Level::Init must be called before getInstance!");
	}
	return *instance;
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
	HUD::getInstance().Update(fElapsedTime);
	enemy->Update(fElapsedTime);
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
			pge->DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, sx, sy, nTileWidth, nTileHeight, 1, 0);
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
					levelY >= startY && levelY < startY + nVisibleTilesY + 1) {
					int screenX = (levelX - startX) * nTileWidth - fTileOffsetX;
					int screenY = (levelY - startY) * nTileHeight - fTileOffsetY - 24;
					sprite->setPosX(screenX);
					sprite->setPosY(screenY);
					sprite->Draw();
				}
			}
		}
	}
	HUD::getInstance().Draw();
	int screenX = (enemy->getHomeX() - startX) * nTileWidth - fTileOffsetX;
	int screenY = (enemy->getHomeY() - startY) * nTileHeight - fTileOffsetY - 27;
	enemy->setPosX(screenX);
	enemy->setPosY(screenY);
	enemy->Draw();
}

bool Level::isMoveable(int x, int y, float velY) {
	// Ensure coordinates are within the level bounds
	if (x < 0 || x >= nLevelWidth || y < 0 || y >= nLevelHeight)
		return false;  // Outside the level bounds, not moveable

	// Get the tile from the main level data
	wchar_t levelTile = GetTile(x, y);

	// Check if the tile is a solid terrain
	bool isLevelTileBlocked = (moveAbleTiles.count(levelTile) == 0);

	// Platform behavior
	if (isPlatform(x, y)) {
		if (velY > 0) {
			// Falling: allow standing on the platform
			return true;
		}
		else {
			std::cout << "into platform" << std::endl;
			// Jumping: allow passing through
			return false;
		}
	}

	// Default moveable check
	return isLevelTileBlocked;
}

bool Level::isMoveable(int x, int y, bool checkPlatformAsSolid)
{
	wchar_t levelTile = GetTile(x, y); // Assuming `getTile` fetches the tile at (x, y)
	if (levelTile == L'R' && !checkPlatformAsSolid)
		return true; // Treat platform as moveable when coming from below
	return moveAbleTiles.count(levelTile) == 0;
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

PixelSprite* Level::checkCollisionWithEnemies(const Rect& playerRect) {
	Rect hitbox = enemy->getRect();
	if (playerRect.intersects(hitbox)) {
		return enemy;
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

bool Level::isPlatform(int x, int y)
{
	if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
	{
		wchar_t decorTile = sDecoration[(y) * nLevelWidth + x];
		return (decorTile == 'Z' || decorTile == 'X' || decorTile == 'C' || decorTile == 'V');
	}
	return false;
}