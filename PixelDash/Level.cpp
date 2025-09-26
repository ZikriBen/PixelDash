#include "Level.h"

Level* Level::instance = nullptr;

Level::Level()
{
	// Empty constructor body; initialization happens in Init
}

void Level::Init(olc::PixelGameEngine& pge)
{
	if (!instance)
	{
		instance = new Level();
		instance->pge = &pge;  // Store the game engine reference
		
		instance->spriteTiles = new olc::Sprite("assets/Terrain32x32.png");
		instance->spriteDoor = new olc::Sprite("assets/IdleDoor.png");

		HUD::init(pge);

		instance->sLevel = LEVEL_DATA;
		instance->sDecoration = DECORATION_DATA;
		instance->tileOffsets = TILE_OFFSETS;
		instance->moveAbleTiles = MOVEABLE_TILES;
		//instance->startSound();
		
		// create decoration array
		for (int x = 0; x < LEVEL_WIDTH; ++x)
		{
			for (int y = 0; y < LEVEL_HEIGHT; ++y)
			{
				wchar_t cDecorID = instance->sDecoration[y * LEVEL_WIDTH + x];
				if (cDecorID == 'D') {
					instance->sDoorOpen = new PixelSprite(pge, "assets/door.png", 4, 0.2f, 46, 56, 0, 0);
					instance->pixelSprites[cDecorID].emplace_back(
						instance->sDoorOpen,
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'B') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Box(pge, "assets/Box.png", 0.0f, 0.0f, 1, 0.2f, 22, 16, 0, 0, 0.0f, 8.0f),
						std::make_pair(x, y)
					);

				}
				else if (cDecorID == 'F') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Flag(pge, FlagType::TWO),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'O') {
					Coin* c = new Coin(pge, "assets/BigDiamond.png", 0.0f, 0.0f, 9, 0.1f, 18, 14, 2, 0, 0.0f, 8.0f);
					c->setAnimation(true);
					c->setLoop(true);
					instance->pixelSprites[cDecorID].emplace_back(
						c,
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'H') {
					Heart* h = new Heart(pge, "assets/BigHeart.png", 0.0f, 0.0f, 7, 0.1f, 18, 14, 2, 0, 0.0f, 8.0f);
					h->setAnimation(true);
					h->setLoop(true);
					instance->pixelSprites[cDecorID].emplace_back(
						h,
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'E') {
					instance->enemy = new Enemy(pge, "assets/Pig.png", (float)x, (float)y, 10, 0.1f, 34, 28, 2, 0, 0.0f, 0.0f);
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
						new Platform(pge, PlatformType::ONE, (float)x, (float)y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'X') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::TWO, (float)x, (float)y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'C') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::THREE, (float)x, (float)y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'V') {
					instance->pixelSprites[cDecorID].emplace_back(
						new Platform(pge, PlatformType::FOUR, (float)x, (float)y),
						std::make_pair(x, y)
					);
				}
				else if (cDecorID == 'Y') {
					instance->cannon = new Cannon(pge, "assets/CannonShoot.png", 0.0f, 0.0f, 2, 0.1f, 44, 28, 44, 0, 0.0f, 0.0f);
					instance->cannon->setAnimation(true);
					instance->cannon->setLoop(true);
					instance->cannon->setHomeX((float)x);
					instance->cannon->setHomeY((float)y);
				}
			}
		}
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
	if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT)
		return sLevel[y * LEVEL_WIDTH + x];
	else
		return L' ';
}

void Level::SetTile(int x, int y, wchar_t c)
{
	if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT)
		sLevel[y * LEVEL_WIDTH + x] = c;
}

void Level::Update(float fElapsedTime)
{
	fTotalTime += fElapsedTime;

	for (const auto& entry : pixelSprites) {
		for (const auto& spritePos : entry.second) {
			PixelSprite* sprite = spritePos.first;
			if (spritePos.first) {
				spritePos.first->Update(fElapsedTime);
			}
		}
	}
	instance->cannon->Update(fElapsedTime);
	HUD::getInstance().Update(fElapsedTime);
	enemy->Update(fElapsedTime);
	HandleTimedSprites(fElapsedTime);
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
			pge->DrawPartialSprite((int32_t)(x * TILE_WIDTH - fTileOffsetX), (int32_t)(y * TILE_HEIGHT - fTileOffsetY), spriteTiles, sx, sy, TILE_WIDTH, TILE_HEIGHT, 1, 0);
		}
	}

	int startX = static_cast<int>(fOffsetX);  // Leftmost visible tile in the level
	int startY = static_cast<int>(fOffsetY);  // Topmost visible tile in the level

	for (const auto& entry : pixelSprites) {
		for (const auto& spritePos : entry.second) {
			PixelSprite* sprite = spritePos.first;
			auto [levelX, levelY] = spritePos.second;

			if (sprite) {
				if (levelX + 1 >= startX && levelX < startX + nVisibleTilesX + 1 &&
					levelY >= startY && levelY < startY + nVisibleTilesY + 1) {
					float screenX = (levelX - startX) * TILE_WIDTH - fTileOffsetX;
					float screenY = (levelY - startY) * TILE_HEIGHT - fTileOffsetY - 24;
					sprite->setPosX(screenX);
					sprite->setPosY(screenY);
					sprite->Draw();
				}
			}
		}
	}
	HUD::getInstance().Draw();

	enemy->Draw((float)startX, (float)startY, fTileOffsetX, fTileOffsetY);
	cannon->Draw((float)startX, (float)startY, fTileOffsetX, fTileOffsetY);

	for (auto sprite : activeSprites) {
		sprite->Draw();
	}
}

bool Level::isMoveable(float x, float y)
{
	return isMoveable((int)x, (int)y);
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

PixelSprite* Level::checkCollisionWithEnemies(const Rect& playerRect) {
	if (cannon->isAlive()) {
		Rect hitbox = cannon->getRect();
		if (playerRect.intersects(hitbox)) {
			return cannon;
		}
	}
	
	if (enemy->isAlive()) {
		Rect hitbox = enemy->getRect();
		if (playerRect.intersects(hitbox)) {
			return enemy;
		}
	}
	return nullptr;
}

PixelSprite* Level::checkCollisionWithProjectiles(const Rect& playerRect) {
	auto projectiles = cannon->getProjectiles();

	for (auto& proj : projectiles) {
		Rect projRect = proj->getRect();
		if (playerRect.intersects(projRect)) {
			return proj;
		}
	}
	return nullptr;
}


void Level::removeDecoration(PixelSprite* decoration) {
	for (auto& entry : pixelSprites) {
		auto& vec = entry.second;
		removeDecoration(entry.first, decoration);
	}
	delete decoration;
}

void Level::removeDecoration(wchar_t key, PixelSprite* decoration) {
	auto it = pixelSprites.find(key);
	if (it != pixelSprites.end()) {
		auto& vec = it->second;
		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[decoration](const std::pair<PixelSprite*, std::pair<int, int>>& item) {
				return item.first == decoration;
			}), vec.end());
	}
	
}

bool Level::isPlatform(float x, float y)
{
	return isPlatform((int) x, (int)y);
}

bool Level::isPlatform(int x, int y)
{
	if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT)
	{
		wchar_t decorTile = sDecoration[(y) *LEVEL_WIDTH + x];
		return (decorTile == 'Z' || decorTile == 'X' || decorTile == 'C' || decorTile == 'V');
	}
	return false;
}

void Level::addTimedSprite(TimedSprite* sprite) {
	spriteQueue.push(sprite);
}

void Level::HandleTimedSprites(float fElapsedTime) {

	while (!spriteQueue.empty() && spriteQueue.top()->getStartTime() <= fTotalTime) {
		activeSprites.push_back(spriteQueue.top());
		spriteQueue.pop(); // Pop and process in one line
	}

	activeSprites.erase(
		std::remove_if(activeSprites.begin(), activeSprites.end(),
			[](TimedSprite* sprite) { return !sprite->isActive(); }),
		activeSprites.end());

	// Update remaining active sprites
	for (TimedSprite* sprite : activeSprites) {
		sprite->Update(fElapsedTime);
	}
}

void Level::startSound() {
	this->nMenuMusic_ID = this->miniAudio.LoadSound(this->souMenuMusic);
	this->miniAudio.SetVolume(this->nMenuMusic_ID, 0.7f);
	this->miniAudio.Play(this->nMenuMusic_ID, true);
}