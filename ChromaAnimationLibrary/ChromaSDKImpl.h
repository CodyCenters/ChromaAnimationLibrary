/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#ifndef _CHROMASDKIMPL_H_
#define _CHROMASDKIMPL_H_
#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include "AnimationManager.h"

namespace ChromaAnimation
{
  #define ALL_DEVICES         0
  #define KEYBOARD_DEVICES    1
  #define MOUSEMAT_DEVICES    2
  #define MOUSE_DEVICES       3
  #define HEADSET_DEVICES     4
  #define KEYPAD_DEVICES      5
  
  enum DEVICES
  {
    ALL = 0,
    KEYBOARD = 1,
    MOUSEPAD = 2,
    MOUSE = 3,
    HEADSET = 4,
    KEYPAD = 5,
  };
  
  struct Animation;
  struct LayeredAnimation;
  
  //Global boolean to message Animation thread to turn off
  extern BOOL EndCurrentRunningAnimation;
  
  class CChromaSDKImpl
  {
  public:
    CChromaSDKImpl();
    ~CChromaSDKImpl();
  
    //Connects to the Chroma SDK
    BOOL Initialize();
    //Disconnects from the Chroma SDK
    BOOL UnInitialize();
    //Checks the validity of the SDK connection
    BOOL IsDeviceConnected(RZDEVICEID DeviceId);
  
    //Clear effects on given device
    void ResetEffects(UINT DeviceType);
    //Begin running the animation thread
    void Run(void);
    //Stop the animation thread
    void StopCurrentAnimations(void);
    //Gets the Manager that handles all things animation
    AnimationManager& GetAnimationManager(void);
  
    //My other test animation, separate thread
    void DoBreath(void);
  private:
    bool m_animationRunning;
    HMODULE m_ChromaSDKModule;
    AnimationManager m_animationManager;
  };
  
  extern CChromaSDKImpl gSDK;
}

#endif
