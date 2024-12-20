#include "olcPixelGameEngine.h"
#include "PixelSprite.h"

PixelSprite::PixelSprite(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
    :pge(pge), iNumFrames(numFrames), fFrameDuration(frameDuration), fWidth((float)width), fHeight((float)height), fOffsetX((float)ox), fOffsetY((float)oy)
{
    spr = new olc::Sprite(sprPath);
    iScale = 1;
    iFlip = 0;
    fGraphicTimer = 0.0f; 
    iGraphicCounter = 0;
    bIsAnimate = false;
    bIsLoop = false;
    bSoundOn = true;
    fCurrentHealth = 0.0f;
    fPosX = 0.0f;
    fPosY = 0.0f;
    eFacingDirection = PixelSprite::RIGHT;
    eGraphicState = AnimationState::IDLE;

    animations = {
        {AnimationState::IDLE, {iNumFrames, (int)fWidth, (int)fHeight, fFrameDuration, (int)fOffsetX, (int)fOffsetY, (int)fPosOffsetX, (int)fPosOffsetY}},
    };

    currentAnimation = animations[eGraphicState];
}

PixelSprite::PixelSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
    :pge(pge), fPosX(posx), fPosY(posy), iNumFrames(numFrames), fFrameDuration(frameDuration), fWidth((float)width), fHeight((float)height), fOffsetX((float)ox), fOffsetY((float)oy), fPosOffsetX(offsetPosX), fPosOffsetY(offsetPosY)
{
    spr = new olc::Sprite(sprPath);
    iScale = 1;
    iFlip = 0;
    fGraphicTimer = 0.0f;
    iGraphicCounter = 0;
    bIsAnimate = false;
    bIsLoop = false;
    bSoundOn = true;
    fCurrentHealth = 0.0f;
    eFacingDirection = PixelSprite::RIGHT;
    eGraphicState = AnimationState::IDLE;
    
    animations = {
        {AnimationState::IDLE, {numFrames, width, height, fFrameDuration, (int)offsetPosY, (int)ox, 0, (int)oy}},
    };

    currentAnimation = animations[eGraphicState];
}

PixelSprite::~PixelSprite()
{
    delete spr;
    spr = nullptr;
}

void PixelSprite::Update(float fElapsedTime)
{
    fGraphicTimer += fElapsedTime;
 
    if (bIsAnimate) {

        if (fGraphicTimer > currentAnimation.frameDuration) {
            iGraphicCounter++;
            iGraphicCounter %= currentAnimation.iNumFrames + 1;
            
            if (!bIsLoop) {
                if (iGraphicCounter >= currentAnimation.iNumFrames) {
                    bIsAnimate = false;
                }
            }
            fGraphicTimer = 0;
        }
    }
    
}

void PixelSprite::Draw()
{
	pge.SetPixelMode(olc::Pixel::MASK);
    pge.DrawPartialSprite(
        (int32_t)(fPosX + fPosOffsetX),
        (int32_t)(fPosY + fPosOffsetY),
        spr,
        currentAnimation.iSprOffsetX + (iGraphicCounter * currentAnimation.frameWidth),
        currentAnimation.iSprOffsetY,
        currentAnimation.frameWidth,
        currentAnimation.frameHeight,
        iScale,
        eFacingDirection);
	pge.SetPixelMode(olc::Pixel::NORMAL);
    //DrawRect();
}

void PixelSprite::DrawRect() {
    pge.SetPixelMode(olc::Pixel::MASK);
    pge.DrawRect(getRect().x, getRect().y, getRect().width, getRect().height, olc::WHITE);
    pge.SetPixelMode(olc::Pixel::NORMAL);
}

Rect PixelSprite::getRect() {
    return Rect(
        (int)(getPosX() + fPosOffsetX),
        (int)(getPosY() + fPosOffsetY),
        (int)getWidth(),
        (int)getHeight()
    );
}

void PixelSprite::setGraphicState(AnimationState state)
{
    eGraphicState = state;  
    currentAnimation = animations[eGraphicState];
}

void PixelSprite::resetGraphicState(AnimationState state)
{
    eGraphicState = state;
    currentAnimation = animations[eGraphicState];
    setGraphicCounter(0);
    setGraphicTimer(0.0f);
}
