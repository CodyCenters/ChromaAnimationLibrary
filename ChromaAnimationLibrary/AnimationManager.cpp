/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "AnimationFileLoader.h"
#include <direct.h>


namespace ChromaAnimation
{
  AnimationManager::AnimationManager()
  {
    //Create the animation folders if they are not there already
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.bInheritHandle = false;
    attr.lpSecurityDescriptor = nullptr;
    int res = CreateDirectory(L"C:\\ProgramData\\ChromaAnimation\\", &attr);
    res = CreateDirectory(L"C:\\ProgramData\\ChromaAnimation\\Animations\\", &attr);
  
    //create the animations arrays for each device
    m_runningAnimations[0] = std::vector<RunningAnimation>();
    m_runningAnimations[1] = std::vector<RunningAnimation>();
    m_runningAnimations[2] = std::vector<RunningAnimation>();
    m_runningAnimations[3] = std::vector<RunningAnimation>();
    m_runningAnimations[4] = std::vector<RunningAnimation>();
    m_runningAnimations[5] = std::vector<RunningAnimation>();
  
    //set the manager ptr for the file systems
    m_fileLoader.m_manager = this;
    m_fileSaver.m_manager = this;
  
    //default FPS
    m_FPS = 30;
  
    //Automatically load certain files
    CheckAutoLoad();
  }
  
  AnimationManager::~AnimationManager()
  {
    //destroy all animations
    Cleanup();
  }
  
  bool AnimationManager::LoadAnimation(const char* file)
  {
    //If not already loaded, load
    if(m_loadedAnimations.find(file) != m_loadedAnimations.end())
      return false;
  
    m_loadedAnimations[file] = m_fileLoader.Load(file);
    return true;
  }
  
  bool AnimationManager::LoadLayeredAnimation(const char* file)
  {
    //If not already loaded, load
    if(m_layeredAnimations.find(file) != m_layeredAnimations.end())
      return false;
  
    auto animations = m_fileLoader.LoadLayered(file);
  
    //load the animations in the layered animation that have not yet been loaded
    for(std::string name : animations)
      if(m_loadedAnimations.find(name) == m_loadedAnimations.end())
        LoadAnimation(name.c_str());
  
    m_layeredAnimations[file] = animations;
    return true;
  }
  
  Animation* AnimationManager::GetAnimation(std::string name)
  {
    if(m_loadedAnimations.find(name) != m_loadedAnimations.end())
      return m_loadedAnimations[name];
    return nullptr;
  }
  
  std::vector<std::string> AnimationManager::GetLayeredAnimation(std::string name)
  {
    if(m_layeredAnimations.find(name) != m_layeredAnimations.end())
      return m_layeredAnimations[name];
    return std::vector<std::string>();
  }
  
  std::vector<RunningAnimation>& AnimationManager::GetRunningAnimationsOnDevice(unsigned device)
  {
    return m_runningAnimations[device];
  }
  
  void AnimationManager::RemoveCompletedAnimations(void)
  {
    //Checks the currently running animations on all devices
    //Removes them if they are done
    std::vector<RunningAnimation>& All = m_runningAnimations[0];
    for(unsigned i = 0; i < All.size(); ++i)
      if(All[0].IsDone())
        All.erase(All.begin() + i--);
  
    std::vector<RunningAnimation>& Keyboard = m_runningAnimations[1];
    for(unsigned i = 0; i < Keyboard.size(); ++i)
      if(Keyboard[i].IsDone())
        Keyboard.erase(Keyboard.begin() + i--);
  
    std::vector<RunningAnimation>& Mousepad = m_runningAnimations[2];
    for(unsigned i = 0; i < Mousepad.size(); ++i)
      if(Mousepad[i].IsDone())
        Mousepad.erase(Mousepad.begin() + i--);
  
    std::vector<RunningAnimation>& Mouse = m_runningAnimations[3];
    for(unsigned i = 0; i < Mouse.size(); ++i)
      if(Mouse[i].IsDone())
        Mouse.erase(Mouse.begin() + i--);
  
    std::vector<RunningAnimation>& Headset = m_runningAnimations[4];
    for(unsigned i = 0; i < Headset.size(); ++i)
      if(Headset[i].IsDone())
        Headset.erase(Headset.begin() + i--);
  
    std::vector<RunningAnimation>& Keypad = m_runningAnimations[5];
    for(unsigned i = 0; i < Keypad.size(); ++i)
      if(Keypad[i].IsDone())
        Keypad.erase(Keypad.begin() + i--);
  }
  
  bool AnimationManager::PlayAnimation(std::string name)
  {
    //Get the animation
    Animation* animation = GetAnimation(name);
    if(animation == nullptr)
      return false;
  
    //find device and create the RunningAnimation
    unsigned devicetype = GetDeviceTypeFromFrameSize(animation->m_frameSize);
    m_runningAnimations[devicetype].push_back(RunningAnimation());
    RunningAnimation& newanim = m_runningAnimations[devicetype].back();
  
    //Set the animation and FPS
    newanim.Set(animation);
    newanim.SetFPS(m_FPS);
  
    //Resort for this device
    SortAnimations(devicetype);
  
    return true;
  }
  
  bool AnimationManager::PlayLayeredAnimation(std::string name)
  {
    //Play each animation in the layered animation
    auto Anims = GetLayeredAnimation(name);
    for(std::string animationname : Anims)
      if(!PlayAnimation(animationname))
        return false;
    return true;
  }
  
  void AnimationManager::CheckAutoLoad(void)
  {
    auto anims = m_fileLoader.LoadLayered("AutoLoadAnimations");
    for(std::string name : anims)
      LoadAnimation(name.c_str());
  }
  
  Animation* AnimationManager::CreateNewAnimation(std::string name, unsigned device, unsigned long framecount)
  {
    //Creates the new animation
    Animation* anim = new Animation();
    //Sets meta data
    anim->m_count = framecount;
    anim->m_frameSize = GetFrameSizeFromDeviceType(device);
    anim->m_loop = false;
    anim->m_static = false;
  
    //creates all the frames
    anim->m_animation = new AnimationFrame[anim->m_count];
    for(unsigned i = 0; i < anim->m_count; ++i)
      anim->m_animation[i] = new unsigned long[anim->m_frameSize];
  
    return anim;
  }
  void AnimationManager::DeleteAnimation(std::string name)
  {
    //deletes all of the frame data and the animation itself
    Animation* anim = m_loadedAnimations[name];
  
    for(unsigned i = 0; i < anim->m_count; ++i)
      delete [] anim->m_animation[i];
    delete [] anim->m_animation;
    delete anim;
  
    //removes from list of loaded animations
    m_loadedAnimations.erase(m_loadedAnimations.find(name));
  }
  
  void AnimationManager::Cleanup(void)
  {
    //deletes all of the animations
    for(std::pair<std::string, Animation*> animation : m_loadedAnimations)
    {
      Animation* anim = animation.second;
  
      for(unsigned i = 0; i < anim->m_count; ++i)
        delete [] anim->m_animation[i];
      delete [] anim->m_animation;
      delete anim;
    }
    //clears all loaded animations
    m_loadedAnimations.clear();
  }
  
  int SortFn(const void* lhs, const void* rhs)
  {
    unsigned Lo,Ll,Ro,Rl;
    RunningAnimation* LHS = ((RunningAnimation*)lhs);
    RunningAnimation* RHS = ((RunningAnimation*)rhs);
    Lo = LHS->GetOuterLayer();
    Ll = LHS->GetLayer();
    Ro = RHS->GetOuterLayer();
    Rl = RHS->GetLayer();
  
    //sort by outer layer first, then by inner layer
    if(Lo != Ro)
      return Lo > Ro;
    else
      return Ll > Rl;
  }
  
  void AnimationManager::SortAnimations(unsigned devicetype)
  {
    std::vector<RunningAnimation>* All = &m_runningAnimations[0];
  
    switch(devicetype)
    {
    case 0:
      All = &m_runningAnimations[0];
      break;
    case 1:
      All = &m_runningAnimations[1];
      break;
    case 2:
      All = &m_runningAnimations[2];
      break;
    case 3:
      All = &m_runningAnimations[3];
      break;
    case 4:
      All = &m_runningAnimations[4];
      break;
    case 5:
      All = &m_runningAnimations[5];
      break;
    }
  
    std::qsort(All->data(), All->size(), sizeof(RunningAnimation), SortFn);
  }
}