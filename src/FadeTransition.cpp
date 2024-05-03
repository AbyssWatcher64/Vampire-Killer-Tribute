#include "FadeTransition.h"

FadeTransition::FadeTransition()
{
    is_active = false;
    from = GameState::MAIN_MENU;
    to = GameState::MAIN_MENU;
    frames_from = 0;
    frames_to = 0;
    frames_counter = 0;
    rect = {};
    state = FadeState::IN;

    w = WINDOW_WIDTH * GAME_SCALE_FACTOR;
    h = WINDOW_HEIGHT * GAME_SCALE_FACTOR;
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };
}
FadeTransition::~FadeTransition()
{
}
void FadeTransition::Set(GameState to, int frames_to, const Rectangle& rect)
{
    is_active = true;

    this->to = to;
    this->frames_to = frames_to;
    frames_counter = 0;
    this->rect = rect;

    state = FadeState::IN;
}
void FadeTransition::Set(GameState from, int frames_from, GameState to, int frames_to, const Rectangle& rect)
{
    is_active = true;

    this->from = from;
    this->to = to;
    this->frames_from = frames_from;
    this->frames_to = frames_to;
    frames_counter = 0;
    this->rect = rect;

    state = FadeState::OUT;
}
 void FadeTransition::SetScene(int to)
{
    // These two look unnecessary
    this->from = GameState::PLAYING;
    this->to = GameState::PLAYING;
    is_active = true;
    this->frames_from = 5;
    this->frames_to = 5;
    this->rect = rect;
    frames_counter = 0;
    state = FadeState::IN;
}
void FadeTransition::SetScene()
{
    this->from = GameState::PLAYING;
    this->to = GameState::PLAYING;
    is_active = true;
    this->frames_from = 5;
    this->frames_to = 5;
    frames_counter = 0;
    this->rect = rect;
    state = FadeState::OUT;
}
bool FadeTransition::IsActive()
{
    return is_active;
}
GameState FadeTransition::Update()
{
    if (is_active)
    {
        frames_counter++;

        //Update the fade state based on the current state
        if (state == FadeState::OUT)
        {
            //Check if it has reached the end
            if (frames_counter >= frames_from)
            {
                state = FadeState::IN;
                frames_counter = 0;
            }
        }
        else if (state == FadeState::IN)
        {
            //Check if it has reached the end
            if (frames_counter >= frames_to)
            {   
                is_active = false; // Transition complete
            }
        }
    }
    //If the fade effect is currently in the "fade-out" state,
    //return the source state (from), otherwise return the target state (to).
    return state == FadeState::OUT ? from : to;
}
void FadeTransition::Render()
{
    float alpha;

    if (is_active)
    {
        if (state == FadeState::OUT)
        {
            //Calculate the alpha value based on the current frame and total frames
            alpha = (float)frames_counter / (float)frames_from;

            //Black rectangle with alpha value from 0 to 1
            //DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(BLACK, alpha));
            //Color color = { 0,0,0,(unsigned char)(alpha * 255.0f) };
            Color color = Fade(BLACK, 255/*alpha*/);
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
        }
        else if (state == FadeState::IN)
        {
            //Calculate the alpha value based on the current frame and total frames
            alpha = (float)frames_counter / (float)frames_to;

            //Black rectangle with alpha value from 1 to 0
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(BLACK, 255/*1-alpha*/));
        }
    }
}
