/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "RunningAnimation.h"
#include "Colors.h"

namespace ChromaAnimation
{
  RunningAnimation::RunningAnimation(): m_animation(nullptr),
                                        m_runTime(0.0f),
                                        m_done(false),
                                        m_fps(0),
                                        m_animationDevice(0)
  {
  }
  RunningAnimation::~RunningAnimation()
  {
  }
  
  bool RunningAnimation::IsSet(void)
  {
    return m_animation != nullptr;
  }
  void RunningAnimation::Set(Animation* anim)
  {
    if(anim == nullptr)
      return;
  
    m_animation = anim;
    m_animationDevice = GetDeviceTypeFromFrameSize(anim->m_frameSize);
  }
  
  unsigned RunningAnimation::GetFPS(void)
  {
    return m_fps;
  }
  void RunningAnimation::SetFPS(unsigned fps)
  {
    m_fps = fps;
  }
  void RunningAnimation::GiveTime(float dt)
  {
    m_runTime += dt;
  }
  unsigned RunningAnimation::GetLayer(void)
  {
    return m_animation->m_layer;
  }
  void RunningAnimation::SetLayer(unsigned layer)
  {
    m_animation->m_layer = layer;
  }
  void RunningAnimation::SetOuterLayer(unsigned layer)
  {
    m_animation->m_outerLayer = layer;
  }
  unsigned RunningAnimation::GetOuterLayer(void)
  {
    return m_animation->m_outerLayer;
  }
  bool RunningAnimation::IsDone(void)
  {
    return m_done;
  }
  unsigned RunningAnimation::GetFrameNumber(void)
  {
    //if the animation is static, or FPS is 0, only show first frame
    if(m_animation->m_static || m_fps <= 0)
      return 0;
    
    //how long the animation is, in seconds
    double animationlength = (double)m_animation->m_count / (double)m_fps;
    //how long we've gone through the animation, in seconds
    double progresstime = ((double)m_fps * (double)m_runTime) / (double)m_fps;
  
    //how complete the animation is [0,1]
    double percentcompletion = progresstime / animationlength;
    //clamp to 1 to prevent invalid indexing
    if(percentcompletion > 1.0)
      percentcompletion = 1.0;
  
    //get which frame in the animation we're on
    double frame = m_animation->m_count * percentcompletion;
    //clamp to prevent overflow
    if(frame >= m_animation->m_count)
    {
      frame = m_animation->m_count - 1;
      //if the animation loops, reset time
      if(m_animation->m_loop)
        m_runTime = 0.0f;
    }
  
    return (unsigned)frame;
  }
  void RunningAnimation::ApplyFrame(void* effect)
  {
    if(effect == nullptr)
      return;
  
    switch(m_animationDevice)
    {
    case 0://all
      break;
    case 1://keyboard
      ApplyKeyboard(reinterpret_cast<ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE*>(effect));
      break;
    case 2://mousepad
      ApplyMousepad(reinterpret_cast<ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE*>(effect));
      break;
    case 3://mouse
      ApplyMouse(reinterpret_cast<ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE*>(effect));
      break;
    case 4://headset
      ApplyHeadset(reinterpret_cast<ChromaSDK::Headset::CUSTOM_EFFECT_TYPE*>(effect));
      break;
    case 5://keypad
      ApplyKeypad(reinterpret_cast<ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE*>(effect));
      break;
    }
  }
  void RunningAnimation::ApplyKeyboard(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* effect)
  {
    //Set each key to the proper color
    //BLACK is our alpha signal
    for(unsigned i = 0; i < ChromaSDK::Keyboard::MAX_ROW; ++i)
      for(unsigned j = 0; j < ChromaSDK::Keyboard::MAX_COLUMN; ++j)
      {
        unsigned frame = GetFrameNumber();
        COLORREF keycolor = m_animation->m_animation[frame][(i * 22) + j];
        if(keycolor != BLACK)
          effect->Color[i][j] = keycolor;
      }
  
    //figure out if we're done
    if(!m_animation->m_loop && GetFrameNumber() == m_animation->m_count - 1)
      m_done = true;
  }
  void RunningAnimation::ApplyKeypad(ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE* effect)
  {
  }
  void RunningAnimation::ApplyMouse(ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE* effect)
  {
  }
  void RunningAnimation::ApplyMousepad(ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE* effect)
  {
  }
  void RunningAnimation::ApplyHeadset(ChromaSDK::Headset::CUSTOM_EFFECT_TYPE* effect)
  {
  }
}