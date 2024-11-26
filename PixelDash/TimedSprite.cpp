
#include "TimedSprite.h"
#include "Level.h"

TimedSprite::TimedSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY, float fStartTime, float fEndTime)
    : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY)
{
    this->fStartTime = fStartTime;
    this->fEndTime = fEndTime;
    this->fTimeOut = std::fabsf(fEndTime - fStartTime);

}
void TimedSprite::Update(float fElapsedTime) {
    fTimePassed += fElapsedTime;
    bActive = !isExpired(fTimePassed);
    
    PixelSprite::Update(fElapsedTime);
}


