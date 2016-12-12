/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once
#include <string>
/*
[Row][Column]
Keyboard: [6][22]
Mouse:    [9][7]
Headset:  [5]
Mousepad: [15]
Keypad:   [4][5]
*/
namespace ChromaAnimation
{
  #define KEYBOARD_FRAME_SIZE 132
  #define MOUSE_FRAME_SIZE 63
  #define HEADSET_FRAME_SIZE 5
  #define MOUSEPAD_FRAME_SIZE 14
  #define KEYPAD_FRAME_SIZE 20
  
  typedef unsigned long* AnimationFrame;
  
  unsigned GetDeviceTypeFromFrameSize(unsigned long size);
  unsigned long GetFrameSizeFromDeviceType(unsigned device);
  DWORD WINAPI Thread_RunAnimation(LPVOID lpParameter);
  
  struct Animation
  {
    //Looping animation or not
    bool m_loop;
    //Static image or not
    bool m_static;
    //Inside layer, used for layered animations
    unsigned m_layer;
    //Main layer component
    unsigned m_outerLayer;
    //Number of frames in the animation
    unsigned long m_count;
    //Size of each frame, can be used to know which device the animation is for
    unsigned long m_frameSize;
    //The raw animation data
    AnimationFrame* m_animation;
  };
  
  //Deprecated
  struct LayeredAnimation
  {
    bool operator==(const LayeredAnimation& rhs){return m_name == rhs.m_name;}
    Animation* m_animationList;
    std::string m_name;
    unsigned m_layer;
  };
}

