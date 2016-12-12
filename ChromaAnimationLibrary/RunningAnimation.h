#pragma once
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "RzErrors.h"
#include "Animation.h"

namespace ChromaAnimation
{
  class RunningAnimation
  {
  public:
    RunningAnimation();
    ~RunningAnimation();
  
    //Apply the correct frame of the animation, to the effect
    void ApplyFrame(void* effect);
    //Update the animation with how much time has passed
    void GiveTime(float dt);
  
    //Whether or not the Amination is set
    bool IsSet(void);
    //Sets the animation
    void Set(Animation* anim);
    //Gets how fast the animation runs
    unsigned GetFPS(void);
    //Sets how fast the animation runs
    void SetFPS(unsigned fps);
    //Gets which inside layer the animation is on
    unsigned GetLayer(void);
    //Sets which inside layer the animation is on
    void SetLayer(unsigned layer);
    //Gets which outer layer the animation is on
    unsigned GetOuterLayer(void);
    //Sets which outer layer the animation is on
    void SetOuterLayer(unsigned layer);
    //Whether the Animation is done or not
    bool IsDone(void);
  
  private:
    Animation* m_animation;
    float m_runTime;
    unsigned m_fps;
    bool m_done;
  
    unsigned m_animationDevice;
  
    void ApplyKeyboard(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* effect);
    void ApplyKeypad(ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE* effect);
    void ApplyMouse(ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE* effect);
    void ApplyMousepad(ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE* effect);
    void ApplyHeadset(ChromaSDK::Headset::CUSTOM_EFFECT_TYPE* effect);
    unsigned GetFrameNumber(void);
  };
}