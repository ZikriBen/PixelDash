
#include "TimedSprite.h"
#include "Level.h"

TimedSprite::TimedSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY, float fTimeout)
    : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY)
{
    this->fTimeout = fTimeout;
}

void TimedSprite::Update(float elapsedTime){
    ftimePassed += elapsedTime;
    PixelSprite::Update(elapsedTime);
}

void TimedSprite::Draw(){
    if (ftimePassed < fTimeout) {
        PixelSprite::Draw();
    }
    else {
        //Level::getInstance().removeDecoration(this);
    }
}

void TimedSprite::setTimeOut(float timeout) {
    this->fTimeout = fTimeout;
}