#pragma once

#include "RenderComponent.h"
#include <vector>

struct Animation
{
    int offset;
    int delay;
    std::vector<Rectangle> frames;
};

class Sprite : public RenderComponent
{
public:
    Sprite(const Texture2D *texture);
    ~Sprite();

    void SetNumberAnimations(int num);
    void SetAnimationDelay(int id, int delay);
    // void SetAnimationOffset(int offset);  //add offset   
    void AddKeyFrame(int id, const Rectangle& rect);
    void SetAnimation(int id);
    
    void Update();
    
    void Draw(int x, int y) const override;
    void DrawTint(int x, int y, const Color& col) const override;

    void Release() override;

private:
    int current_anim;
    int current_frame;
    int current_delay;

    const Texture2D *img;
    std::vector<Animation> animations;
};


//
//f(int a, int b, int x = 0)
//{
//    ...
//}
//
//f();
//f(100);
//
//f(1, 2);
//f(1, 2, 100);