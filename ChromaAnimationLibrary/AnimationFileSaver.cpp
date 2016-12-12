/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "AnimationFileSaver.h"
#include "Animation.h"


namespace ChromaAnimation
{
  FileSaver::FileSaver(const char* name, Animation* anim)
  {
    Save(name, anim);
  }
  
  void FileSaver::Save(const char* name, Animation* anim)
  {
    //Make the file, or open it
    FILE* file = nullptr;
    char fullpath[256] = {0};
    sprintf(fullpath, "C:\\ProgramData\\ChromaAnimation\\Animations\\%s.caf", name);
    file = fopen(fullpath, "wb");
  
    ASSERT(file == nullptr);
  
    //get how many bytes we're writing
    size_t count = anim->m_count * anim->m_frameSize;
  
    //Write the file
    size_t written = fwrite(&anim->m_loop, sizeof(bool), 1, file);
    ASSERT(written != 1);
    written = fwrite(&anim->m_static, sizeof(bool), 1, file);
    ASSERT(written != 1);
    written = fwrite(&anim->m_layer, sizeof(unsigned), 1, file);
    ASSERT(written != 1);
    written = fwrite(&anim->m_outerLayer, sizeof(unsigned), 1, file);
    ASSERT(written != 1);
    written = fwrite(&anim->m_count, sizeof(unsigned long), 1, file);
    ASSERT(written != 1);
    written = fwrite(&anim->m_frameSize, sizeof(unsigned long), 1, file);
    ASSERT(written != 1);
    for(unsigned i = 0; i < anim->m_count; ++i)
    {
      written = fwrite(anim->m_animation[i], sizeof(unsigned long), anim->m_frameSize, file);
      ASSERT(written != anim->m_frameSize);
    }
  
    fclose(file);
  }
}