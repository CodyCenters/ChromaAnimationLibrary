/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once
namespace ChromaAnimation
{
  struct Animation;
  class AnimationManager;
  
  class FileSaver
  {
  public:
    FileSaver(){}
    FileSaver(const char* name, Animation* anim);
    void Save(const char* name, Animation* anim);
    AnimationManager* m_manager;
  };
}