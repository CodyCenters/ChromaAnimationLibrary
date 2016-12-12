/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once
#include <string>
#include <vector>

namespace ChromaAnimation
{
  struct Animation;
  struct LayeredAnimation;
  class AnimationManager;
  
  class FileLoader
  {
  public:
    FileLoader(){}
    ~FileLoader(){}
  
    Animation* Load(const char* file);
    std::vector<std::string> LoadLayered(const char* file);
    AnimationManager* m_manager;
  };
}