#pragma once

#include "RenderComponent.h"
#include <vector>

enum class AnimMode { AUTOMATIC, MANUAL, PLAYONCE };

// struct frame
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
    void AddKeyFrame(int id, const Rectangle& rect);
    void AddKeyFrameOffset(int id, const Rectangle& rect, int offset);
    void SetAnimation(int id);
    int GetAnimation();

    bool GetIsAnimationFinished() const;
    void SetIsAnimationFinished(bool value);

    void SetManualMode();
    void SetAutomaticMode();
    void SetPlayOnceMode();
    bool IsAnimationComplete() const;
    
    void Update();
    void NextFrame();
    void PrevFrame();

    void Draw(int x, int y) const override;
    void DrawTint(int x, int y, const Color& col) const override;
    void Transparent(int x, int y) const override;

    void Release() override;
    
private:
    int current_anim;
    int current_frame;
    int current_delay;
    bool isAnimationFinished;

    const Texture2D *img;
    std::vector<Animation> animations;

    bool animation_complete;

    AnimMode mode;
};

