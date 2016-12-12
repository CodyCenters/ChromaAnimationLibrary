/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once
#include <Windows.h>
#include <unordered_map>
#include <direct.h>

namespace ChromaAnimation
{
  struct Animation;
}
using namespace ChromaAnimation;
class AnimationCreator
{
public:
  AnimationCreator()
  {
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.bInheritHandle = false;
    attr.lpSecurityDescriptor = nullptr;
    int res = CreateDirectory(L"C:\\ProgramData\\ChromaAnimation\\", &attr);
    res = CreateDirectory(L"C:\\ProgramData\\ChromaAnimation\\Animations\\", &attr);
  }
  ~AnimationCreator(){DeleteAllAnimations();}

  Animation* CreateNewAnimation(unsigned device, unsigned long framecount);
  void DeleteAnimation(Animation* anim);

  static AnimationCreator& Get(void)
  {
    static AnimationCreator instance;
    return instance;
  }

private:
  void DeleteAllAnimations(void);
  std::unordered_map<unsigned long, Animation*> m_animations;
};