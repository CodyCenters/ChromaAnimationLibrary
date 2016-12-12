/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "Animation.h"

namespace ChromaAnimation
{
  unsigned GetDeviceTypeFromFrameSize(unsigned long size)
  {
    return (size == 132 ? 1 : size == 63 ? 3 : size == 5 ? 4 : size == 14 ? 2 : size == 20 ? 5 : 0);
  }
  
  unsigned long GetFrameSizeFromDeviceType(unsigned device)
  {
    return (device == 0 ? 132 : device == 1 ? 132 : device == 2 ? 14 : device == 3 ? 63 : device == 4 ? 5 : 20);
  }
}