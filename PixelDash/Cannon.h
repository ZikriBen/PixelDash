
#include "PixelSprite.h"
#include "Projectile.h"

#pragma once
class Cannon : public PixelSprite {
private:
    olc::Sprite* shootSpr;
    PixelSprite* pigSpr;
    float shootCooldown;      // Time between shots
    float lastShootTime = 0.0f;;      // Last time a shot was fired
    std::vector<Projectile*> projectiles; // Active projectiles
    bool isFacingLeft;        // Direction of the cannon
    float fTotalTime = 0.0f;
    bool bIsShooting = false;
    float homeX;
    float homeY;
    int health = 3;
    float lastHitTime = -1.0;
    const float cooldownTime = 0.05f;  
    const float initialVelX = 6.0f;  
    const float initialVelY = -4.0f;  


public:
    Cannon(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY) {
        shootSpr = new olc::Sprite("assets/CannonShoot.png");
        pigSpr = new PixelSprite(pge, "assets/PigLightingtheCannon.png", getPosX(), getPosY(), 7, 0.1f, 26, 18, 26, 0, 39, 7);
        homeX = 0.0f;
        homeY = 0.0f;
        isFacingLeft = false;

        this->animations = {
            {AnimationState::IDLE, {0, 44, 28, 0.1f, 0, 0, 0, 0}},
            {AnimationState::ATTACK, {2, 44, 28, 0.1f, 44, 0, 0, 0}},
        };
        
        std::unordered_map<PixelSprite::AnimationState, PixelSprite::Animation> pigAnimations = {
            {PixelSprite::AnimationState::IDLE, {6, 26, 18, 0.1f, 0, 0, 0, 0}},
            {PixelSprite::AnimationState::HIT, {1, 34, 18, 0.1f, 2, 0, 0, 18}},
            {PixelSprite::AnimationState::DEAD, {3, 34, 18, 0.1f, 2, 0, 0, 36}}
        };
        eFacingDirection = PixelSprite::RIGHT;
        setGraphicState(AnimationState::IDLE);

        pigSpr->setAnimations(pigAnimations);
        pigSpr->setGraphicState(PixelSprite::AnimationState::IDLE);
        pigSpr->setAnimation(true);
        pigSpr->setLoop(true);

        shootCooldown = 9 * frameDuration;
    }

    void Update(float fElapsedTime) override {
        PixelSprite::Update(fElapsedTime);
        pigSpr->setPosX(getPosX());
        pigSpr->setPosY(getPosY());
        pigSpr->Update(fElapsedTime);
        fTotalTime += fElapsedTime;
        
        if (pigSpr->eGraphicState == AnimationState::DEAD) {
            pigSpr->setLoop(false);
        }

        else if (pigSpr->eGraphicState == AnimationState::HIT) {
            // Wait until HIT animation finishes before returning to IDLE
            if (pigSpr->getGraphicCounter() >= currentAnimation.iNumFrames) {
                pigSpr->setGraphicState(AnimationState::IDLE);
            }
        }

        if (pigSpr->eGraphicState == AnimationState::IDLE && fTotalTime - lastShootTime >= shootCooldown) {
            shootProjectile();
            lastShootTime = fTotalTime;
        }
        
        if (bIsShooting) {
            if (getGraphicCounter() >= currentAnimation.iNumFrames) {
                
                bIsShooting = false;
                setGraphicCounter(0); 
                setGraphicState(AnimationState::IDLE); 
            }
        }
        else
            setGraphicState(AnimationState::IDLE);

        // Update all active projectiles
        for (auto& proj : projectiles) {
            proj->Update(fElapsedTime);
        }

        // Remove projectiles that are out of bounds or have collided
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [](Projectile* proj) {
                if (!proj->isActive()) {
                    delete proj;
                    return true;
                }
                return false;
            }), projectiles.end());
    }

    void Draw(float startX, float startY, float fTileOffsetX, float fTileOffsetY) {
		float screenX = (getHomeX() - startX) * TILE_WIDTH - fTileOffsetX;
        float screenY = (getHomeY() - startY) * TILE_HEIGHT - fTileOffsetY - 27;
	    setPosX(screenX);
	    setPosY(screenY);
        PixelSprite::Draw();
        pigSpr->Draw();
        for (auto& proj : projectiles) {
            proj->Draw(startX, startY, fTileOffsetX, fTileOffsetY);
        }
    }

    void shootProjectile() {
        bIsShooting = true;
        setSpr(shootSpr);
        setGraphicState(AnimationState::ATTACK);
        projectiles.push_back(new Projectile(pge, getPosX(), getPosY(), ((eFacingDirection == PixelSprite::LEFT) ? 6.0f : -6.0f), initialVelY, homeX, homeY));
    }
	
    void setHomeX(float homex) {
		homeX = homex;
	}

	void setHomeY(float homey) {
		homeY = homey;
	}

	float getHomeX() {
		return homeX;
	}

    float getHomeY() {
        return homeY;
	}

    std::vector<Projectile*> getProjectiles() { return projectiles; };

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        pigSpr->resetGraphicState(AnimationState::HIT);

        if (!isAlive()) {
            pigSpr->resetGraphicState(AnimationState::DEAD);
        }
    }

    void hit(float currentTime) {
        // Only respond to hit if the cooldown has passed
        if (!isAlive()) {
            return;
        }
        if (currentTime - lastHitTime >= cooldownTime) {
            lastHitTime = currentTime;  // Update the last hit time
            std::cout << "Cannon hit!" << std::endl;
            takeDamage(1);
        }
    }

    virtual Rect getRect() override {
        return pigSpr->getRect();
    }
};

