/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "AnimationManager.h"
#include "AnimationFileLoader.h"


namespace ChromaAnimation
{
  Animation* FileLoader::Load(const char* file)
  {
    //open file
    FILE* Anim = nullptr;
    char fullpath[256] = {0};
    sprintf(fullpath, "C:\\ProgramData\\ChromaAnimation\\Animations\\%s.caf", file);
    Anim = fopen(fullpath, "rb");
    ASSERT(Anim == nullptr);
  
    //read preliminary data
    unsigned long framecount = 0;
    unsigned long framesize = 0;
    unsigned layer = 0;
    unsigned outerlayer = 0;
    bool loop = false;
    bool statik = false;
  
    size_t read = fread(&loop, sizeof(bool), 1, Anim);
    ASSERT(read != 1);
    read = fread(&statik, sizeof(bool), 1, Anim);
    ASSERT(read != 1);
    read = fread(&layer, sizeof(unsigned), 1, Anim);
    ASSERT(read != 1);
    read = fread(&outerlayer, sizeof(unsigned), 1, Anim);
    ASSERT(read != 1);
    read = fread(&framecount, sizeof(unsigned long), 1, Anim);
    ASSERT(read != 1);
    read = fread(&framesize, sizeof(unsigned long), 1, Anim);
    ASSERT(read != 1);
  
    //Create animation
    unsigned DeviceType = GetDeviceTypeFromFrameSize(framesize);
    Animation* animation = m_manager->CreateNewAnimation(file, DeviceType, framecount);
  
    animation->m_loop = loop;
    animation->m_static = statik;
    animation->m_layer = layer;
    animation->m_outerLayer = outerlayer;
  
    //read animation data
    unsigned long animationsize = framecount * framesize;
    for(unsigned i = 0; i < framecount; ++i)
    {
      read = fread(animation->m_animation[i], sizeof(unsigned long), framesize, Anim);
      ASSERT(read != framesize);
    }
  
    return animation;
  }
  
  std::vector<std::string> FileLoader::LoadLayered(const char* file)
  {
    std::vector<std::string> names;
  
    //pull each name from file
  
    return names;
  }
}