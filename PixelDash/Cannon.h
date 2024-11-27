
#include "PixelSprite.h"
#include "Projectile.h"

#pragma once
class Cannon : public PixelSprite {
private:
    olc::Sprite* shootSpr;
    PixelSprite* pigSpr;
    float shootCooldown;      // Time between shots
    float lastShootTime;      // Last time a shot was fired
    std::vector<Projectile*> projectiles; // Active projectiles
    bool isFacingLeft;        // Direction of the cannon
    float fTotalTime = 0.0f;
    bool bIsShooting = false;

public:
    Cannon(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY),
        shootCooldown(9*0.1f), lastShootTime(0.0f) {
        eFacingDirection = PixelSprite::RIGHT;
        shootSpr = new olc::Sprite("assets/CannonShoot.png");
        pigSpr = new PixelSprite(pge, "assets/PigLightingtheCannon.png", getPosX(), getPosY(), 7, 0.1f, 26, 18, 26, 0, 39, 7);
        
        pigSpr->setLoop(true);
        pigSpr->setAnimation(true);
        animations = {
            {AnimationState::IDLE, {0, 44, 28, 0.1, 0, 0, 0, 0}},
            {AnimationState::ATTACK, {2, 44, 28, 0.1, 44, 0, 0, 0}},
        };

        setGraphicState(AnimationState::IDLE);
    }

    void Update(float fElapsedTime) {
        PixelSprite::Update(fElapsedTime);
        pigSpr->setPosX(getPosX());
        pigSpr->setPosY(getPosY());
        pigSpr->Update(fElapsedTime);
        fTotalTime += fElapsedTime;

        if (fTotalTime - lastShootTime >= shootCooldown) {
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

    void Draw() {
        PixelSprite::Draw();
        pigSpr->Draw();
        for (auto& proj : projectiles) {
            proj->Draw();
        }
    }

    void shootProjectile() {
        // Create a new projectile with an initial velocity
        bIsShooting = true;
        setSpr(shootSpr);
        setGraphicState(AnimationState::ATTACK);
        float initialVelX = eFacingDirection == PixelSprite::LEFT ? 250.0f : -250.0f;
        float initialVelY = -80.0f; // Launch upward
        projectiles.push_back(new Projectile(pge, getPosX(), getPosY(), initialVelX, initialVelY));
    }

};