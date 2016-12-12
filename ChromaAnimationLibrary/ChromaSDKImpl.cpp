/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "ChromaSDKImpl.h"
#include "Colors.h"
#include "Animation.h"
#include "AnimationFileLoader.h"
#include "AnimationManager.h"
#include "FrameRateController.h"
#include <vector>

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

namespace ChromaAnimation
{
  typedef RZRESULT (*INIT)(void);
  typedef RZRESULT (*UNINIT)(void);
  typedef RZRESULT (*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
  typedef RZRESULT (*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
  typedef RZRESULT (*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
  typedef RZRESULT (*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
  typedef RZRESULT (*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
  typedef RZRESULT (*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
  typedef RZRESULT (*SETEFFECT)(RZEFFECTID EffectId);
  typedef RZRESULT (*DELETEEFFECT)(RZEFFECTID EffectId);
  typedef RZRESULT (*REGISTEREVENTNOTIFICATION)(HWND hWnd);
  typedef RZRESULT (*UNREGISTEREVENTNOTIFICATION)(void);
  typedef RZRESULT (*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);
  
  INIT Init = NULL;
  UNINIT UnInit = NULL;
  CREATEEFFECT CreateEffect = NULL;
  CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
  CREATEMOUSEEFFECT CreateMouseEffect = NULL;
  CREATEHEADSETEFFECT CreateHeadsetEffect = NULL;
  CREATEMOUSEPADEFFECT CreateMousepadEffect = NULL;
  CREATEKEYPADEFFECT CreateKeypadEffect = NULL;
  SETEFFECT SetEffect = NULL;
  DELETEEFFECT DeleteEffect = NULL;
  QUERYDEVICE QueryDevice = NULL;
  
  BOOL EndCurrentRunningAnimation = 0;
  
  CChromaSDKImpl gSDK;
  
  DWORD WINAPI AnimationThread(LPVOID Params)
  {
    AnimationManager& AnimationMan = gSDK.GetAnimationManager();
    FrameRateController frc(AnimationMan.GetFPS());
  
    ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE KeyboardEffect = {};
    ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE KeypadEffect = {};
    ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE MouseEffect = {};
    ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE MousepadEffect = {};
    ChromaSDK::Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};
  
    while(1)
    {
      frc.OnFrameStart();
  
      //Remove the animations that are done
      AnimationMan.RemoveCompletedAnimations();
  
      //check if we should still be running
      if(EndCurrentRunningAnimation)
      {
        EndCurrentRunningAnimation = 0;
        return 0;
      }
  
      //Run through each devices, and build the effect
      if(CreateKeyboardEffect)
      {
        auto& Animations = AnimationMan.GetRunningAnimationsOnDevice(DEVICES::KEYBOARD);
        if(Animations.size() > 0)
        {
          for(RunningAnimation& CurrentAnimation : Animations)
          {
            CurrentAnimation.ApplyFrame((void*)(&KeyboardEffect));
            CurrentAnimation.GiveTime(frc.GetDT());
          }
          CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &KeyboardEffect, nullptr);
        }
        else
          CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
      }
      if(CreateMousepadEffect)
      {
        auto& Animations = AnimationMan.GetRunningAnimationsOnDevice(DEVICES::MOUSEPAD);
        if(Animations.size() > 0)
        {
          for(RunningAnimation CurrentAnimation : Animations)
          {
            CurrentAnimation.ApplyFrame((void*)(&KeyboardEffect));
            CurrentAnimation.GiveTime(frc.GetDT());
          }
          CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM, &MousepadEffect, nullptr);
        }
        else
          CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, nullptr, nullptr);
      }
      if(CreateMouseEffect)
      {
        auto& Animations = AnimationMan.GetRunningAnimationsOnDevice(DEVICES::MOUSE);
        if(Animations.size() > 0)
        {
          for(RunningAnimation CurrentAnimation : Animations)
          {
            CurrentAnimation.ApplyFrame((void*)(&KeyboardEffect));
            CurrentAnimation.GiveTime(frc.GetDT());
          }
          CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM, &MouseEffect, nullptr);
        }
        else
          CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, nullptr, nullptr);
      }
      if(CreateHeadsetEffect)
      {
        auto& Animations = AnimationMan.GetRunningAnimationsOnDevice(DEVICES::HEADSET);
        if(Animations.size() > 0)
        {
          for(RunningAnimation CurrentAnimation : Animations)
          {
            CurrentAnimation.ApplyFrame((void*)(&KeyboardEffect));
            CurrentAnimation.GiveTime(frc.GetDT());
          }
          CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_CUSTOM, &HeadsetEffect, nullptr);
        }
        else
          CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, nullptr, nullptr);
      }
      if(CreateKeypadEffect)
      {
        auto& Animations = AnimationMan.GetRunningAnimationsOnDevice(DEVICES::KEYPAD);
        if(Animations.size() > 0)
        {
          for(RunningAnimation CurrentAnimation : Animations)
          {
            CurrentAnimation.ApplyFrame((void*)(&KeyboardEffect));
            CurrentAnimation.GiveTime(frc.GetDT());
          }
          CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_CUSTOM, &KeypadEffect, nullptr);
        }
        else
          CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, nullptr, nullptr);
      }
  
      frc.OnFrameEnd();
    }
  
    return 0;
  }
  
  //Interpolate from Black to a color [0,1]
  COLORREF InterpolateFromBlack(COLORREF base, float t)
  {
    BYTE R = (BYTE)(GetRValue(base) * t);
    BYTE G = (BYTE)(GetGValue(base) * t);
    BYTE B = (BYTE)(GetBValue(base) * t);
    return RGB(R,G,B);
  }
  
  //Breath effect for Razer Technical Test
  DWORD WINAPI BreatheEffect(LPVOID params)
  {
    if(CreateKeyboardEffect)
    {
      ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect = {};
      FrameRateController FRC(60);//60 FPS
      float Time = 0.0f;
  
      COLORREF BreathColors[2] = { RED, GREEN };
      unsigned ColorIndex = 0;
      COLORREF CurrentColor = RED;
      COLORREF ColorVal = RED;
      float PreviousInterpolant = 0.0f;
  
      while(true)
      {
        FRC.OnFrameStart();
  
        float Interpolant = sinf(Time);
  
        if(Interpolant <= 0.0f)
        {
          if(PreviousInterpolant > 0.0f)
            CurrentColor = BreathColors[++ColorIndex % 2];
          ColorVal = BLACK;
        }
        else
          ColorVal = InterpolateFromBlack(CurrentColor, Interpolant);
  
  
        for(unsigned c = 0; c < 22; ++c)
        {
          for(unsigned r = 0; r < 6; ++r)
          {
            Effect.Color[r][c] = ColorVal;
          }
        }
  
        CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, nullptr);
        PreviousInterpolant = Interpolant;
  
        FRC.OnFrameEnd();
        Time += FRC.GetDT();
      }
    }
    return 0;
  }
  
  CChromaSDKImpl::CChromaSDKImpl():m_ChromaSDKModule(NULL)
  {
  }
  
  CChromaSDKImpl::~CChromaSDKImpl()
  {
    StopCurrentAnimations();
  }
  
  BOOL CChromaSDKImpl::Initialize()
  {
    //Load the chroma SDK dll, if we havent
    if(m_ChromaSDKModule == NULL)
    {
      m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
      //if that failed, error out
      if(m_ChromaSDKModule == NULL)
      {
        ASSERT(GetLastError() == ERROR_SUCCESS);
        return FALSE;
      }
    }
  
    //if we dont have the Init function for SDK
    if(Init == NULL)
    {
      //try get it
      RZRESULT Result = RZRESULT_INVALID;
      Init = (INIT)GetProcAddress(m_ChromaSDKModule, "Init");
      //if we got it
      if(Init)
      {
        //gogo
        Result = Init();
        if(Result == RZRESULT_SUCCESS)
        {
          //pull the needed functions
          CreateEffect = (CREATEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateEffect");
          CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect");
          CreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMouseEffect");
          CreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateHeadsetEffect");
          CreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMousepadEffect");
          CreateKeypadEffect = (CREATEKEYPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeypadEffect");
          SetEffect = (SETEFFECT)GetProcAddress(m_ChromaSDKModule, "SetEffect");
          DeleteEffect = (DELETEEFFECT)GetProcAddress(m_ChromaSDKModule, "DeleteEffect");
          QueryDevice = (QUERYDEVICE)GetProcAddress(m_ChromaSDKModule, "QueryDevice");
  
          //if we got them all, we're good
          if(CreateEffect &&
              CreateKeyboardEffect &&
              CreateMouseEffect &&
              CreateHeadsetEffect &&
              CreateMousepadEffect &&
              CreateKeypadEffect &&
              SetEffect &&
              DeleteEffect &&
              QueryDevice)
          {
            return TRUE;
          }
          else//if we didnt, fail out
          {
            return FALSE;
          }
        }
      }
    }
  
    return TRUE;
  }
  
  BOOL CChromaSDKImpl::UnInitialize()
  {
    if(m_ChromaSDKModule != NULL)
    {
      //get uninit function, and do it
      RZRESULT Result = RZRESULT_INVALID;
      UNINIT UnInit = (UNINIT)GetProcAddress(m_ChromaSDKModule, "UnInit");
      if(UnInit)
      {
        Result = UnInit();
        ASSERT(Result == RZRESULT_SUCCESS);
      }
      //release the SDK
      FreeLibrary(m_ChromaSDKModule);
      m_ChromaSDKModule = NULL;
  
      return TRUE;
    }
  
    return FALSE;
  }
  
  void CChromaSDKImpl::ResetEffects(UINT DeviceType)
  {
    //clear all effects on given device
    switch(DeviceType)
    {
    case 0:
      if(CreateKeyboardEffect)
        CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, NULL, NULL);
      if(CreateMousepadEffect)
        CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, NULL, NULL);
      if(CreateMouseEffect)
        CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, NULL, NULL);
      if(CreateHeadsetEffect)
        CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, NULL, NULL);
      if(CreateKeypadEffect)
        CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, NULL, NULL);
      break;
    case 1:
      if(CreateKeyboardEffect)
        CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, NULL, NULL);
      break;
    case 2:
      if(CreateMousepadEffect)
        CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, NULL, NULL);
      break;
    case 3:
      if(CreateMouseEffect)
        CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, NULL, NULL);
      break;
    case 4:
      if(CreateHeadsetEffect)
        CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, NULL, NULL);
      break;
    case 5:
      if(CreateKeypadEffect)
        CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, NULL, NULL);
      break;
    }
  }
  
  BOOL CChromaSDKImpl::IsDeviceConnected(RZDEVICEID DeviceId)
  {
    //check if device is good
    if(QueryDevice != NULL)
    {
      ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
      RZRESULT Result = QueryDevice(DeviceId, DeviceInfo);
  
      ASSERT(Result == RZRESULT_SUCCESS);
  
      return DeviceInfo.Connected;
    }
  
    return FALSE;
  }
  
  void CChromaSDKImpl::StopCurrentAnimations()
  {
    //msg animation thread to stop running
    if(m_animationRunning)
    {
      EndCurrentRunningAnimation = 1;
      while(EndCurrentRunningAnimation)
        Sleep(50);
  
      m_animationRunning = false;
    }
  }
  
  void CChromaSDKImpl::Run(void)
  {
  	HANDLE hWorkerThread = NULL;
    hWorkerThread = CreateThread(NULL, 0, AnimationThread, nullptr, 0, NULL);
    m_animationRunning = true;
  	CloseHandle(hWorkerThread);
  }
  
  void CChromaSDKImpl::DoBreath(void)
  {
    HANDLE hThread = nullptr;
    hThread = CreateThread(nullptr, 0, BreatheEffect, nullptr, 0, nullptr);
    CloseHandle(hThread);
  }
  AnimationManager& CChromaSDKImpl::GetAnimationManager(void)
  {
    return m_animationManager;
  }
}