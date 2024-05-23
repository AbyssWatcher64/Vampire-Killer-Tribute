#include "Sprite.h"

Sprite::Sprite(const Texture2D * texture)
{
    img = texture;
    isAnimationFinished = false;
    current_anim = -1;
    current_frame = 0;
    current_delay = 0;
    mode = AnimMode::AUTOMATIC; // TODO: Revise if this is good coding, since we have the...
    //...play only one cinematic
    //animations[current_anim].offset = 0;
    animation_complete = false;
}
Sprite::~Sprite()
{
    Release();
}
void Sprite::SetNumberAnimations(int num)
{
    animations.clear();
    animations.resize(num);
}
void Sprite::SetAnimationDelay(int id, int delay)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].delay = delay;
    }
}
void Sprite::AddKeyFrame(int id, const Rectangle& rect)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].frames.push_back(rect);
    }
}
void Sprite::AddKeyFrameOffset(int id, const Rectangle& rect, int offset)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].offset = offset;
        animations[id].frames.push_back(rect);
    }
}
void Sprite::SetAnimation(int id)
{
    if (id >= 0 && id < animations.size())
    {
        current_anim = id;
        current_frame = 0;
        current_delay = animations[current_anim].delay;
        animation_complete = false;
    }
}
int Sprite::GetAnimation()
{
    return current_anim;
}
bool Sprite::GetIsAnimationFinished() const
{
    return isAnimationFinished;
}
void Sprite::SetIsAnimationFinished(bool value)
{
    isAnimationFinished = value;
}
void Sprite::SetManualMode()
{
    mode = AnimMode::MANUAL;
}
void Sprite::SetAutomaticMode()
{
    mode = AnimMode::AUTOMATIC;
}
bool Sprite::IsAnimationComplete() const
{
    return animation_complete;
}
void Sprite::SetPlayOnceMode()
{
    mode = AnimMode::PLAYONCE;
    SetIsAnimationFinished(false);
}
void Sprite::Update()
{
    //Both animation modes (automatic and manual) are carry out with animation delay
    if (current_delay > 0)
    {
        current_delay--;
        if (current_delay == 0)
        {
            //Only automatic animation mode advances next frame
            if (mode == AnimMode::AUTOMATIC)
            {
                current_frame++;
                current_frame %= animations[current_anim].frames.size();
                current_delay = animations[current_anim].delay;

                //Animation is complete when we repeat from the first frame
                animation_complete = (current_frame == 0);
            }

            //TODO: Make this work
            if (mode == AnimMode::PLAYONCE)
            {

                current_frame++;
                //current_delay = animations[current_anim].delay;
                current_delay = animations[current_anim].delay;
                //current_delay = animations[current_anim].delay;
                //if (current_frame == animations[current_anim].frames.size() - 1)
                if (current_frame == animations[current_anim].frames.size())
                {    
                        SetIsAnimationFinished(true);
                        mode = AnimMode::AUTOMATIC;
                }
            }
        }
    }
}


void Sprite::NextFrame()
{
    //Next frame is only available in manual animation mode
    if (mode == AnimMode::MANUAL)
    {
        current_delay--;
        if (current_delay <= 0)
        {
            current_frame++;
            current_frame %= animations[current_anim].frames.size();
            current_delay = animations[current_anim].delay;
        }
    }   
}
void Sprite::PrevFrame()
{
    //Previous frame is only available in manual animation mode
    if (mode == AnimMode::MANUAL)
    {
        current_delay--;
        if (current_delay <= 0)
        {
            current_frame--;
            current_frame %= animations[current_anim].frames.size();
            current_delay = animations[current_anim].delay;
        }
    }
}
void Sprite::Draw(int x, int y) const
{
    DrawTint(x, y, WHITE);
}

void Sprite::DrawTint(int x, int y, const Color& col) const
{
    if (current_anim >= 0 && current_anim < animations.size())
    {
        int n = animations[current_anim].frames.size();
        if (current_frame >= 0 && current_frame < n)
        {
            Rectangle rect = animations[current_anim].frames[current_frame];
            int offset = animations[current_anim].offset; // This wasn't in this framework
            DrawTextureRec(*img, rect, { (float)x + offset, (float)y }, col);
        }
    }
}
void Sprite::Transparent(int x, int y) const
{
    DrawTint(x, y, {0,0,0,0});
}

void Sprite::Release()
{
    //Release each animation's frames
    for (auto& animation : animations)
    {
        animation.frames.clear();
    }
    //Clear the animations vector
    animations.clear();
}
