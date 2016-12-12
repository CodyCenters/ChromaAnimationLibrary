/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "..\ChromaAnimationLibrary\ChromaAnimationLibrary.h"
#include "AnimationCreation.h"

using namespace ChromaAnimation;
Animation* AnimationCreator::CreateNewAnimation(unsigned device, unsigned long framecount)
{
  Animation* anim = new Animation();
  m_animations[reinterpret_cast<unsigned long>(anim)] = anim;

  anim->m_count = framecount;
  anim->m_frameSize = GetFrameSizeFromDeviceType(device);
  anim->m_loop = false;
  anim->m_static = false;


  anim->m_animation = new AnimationFrame[anim->m_count];
  for(unsigned i = 0; i < anim->m_count; ++i)
    anim->m_animation[i] = new unsigned long[anim->m_frameSize];

  return anim;
}
void AnimationCreator::DeleteAnimation(Animation* animation)
{
  Animation* anim = m_animations[reinterpret_cast<unsigned long>(animation)];

  for(unsigned i = 0; i < anim->m_count; ++i)
    delete [] anim->m_animation[i];
  delete [] anim->m_animation;
  delete anim;
}
void AnimationCreator::DeleteAllAnimations(void)
{
  while(!m_animations.empty())
  {
    DeleteAnimation(m_animations.begin()->second);
    m_animations.erase(m_animations.begin());
  }
}