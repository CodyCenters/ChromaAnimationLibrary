/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "stdafx.h"
#include "FrameRateController.h"

namespace ChromaAnimation
{
  #define FRAMES_TO_TRACK 120
  #define TO_MILLISECONDS(sec) ((unsigned long)(sec * 1000))
  #define TO_SECONDS(milli) (milli / 1000.0f)
  
  
  FrameRateController::FrameRateController(unsigned fps): m_targetFPS(fps), 
                                                          m_frameIterator(0),
                                                          m_pastTrackedAmount(false)
  {
    //See how long each frame should take, in milliseconds
    m_targetDT_InMilli = TO_MILLISECONDS((1.0f / (float)m_targetFPS));
  
    m_previousFrames.reserve(FRAMES_TO_TRACK);
    for(int i = 0; i < FRAMES_TO_TRACK; ++i)
      m_previousFrames.push_back(0);
  }
  
  void FrameRateController::OnFrameStart()
  {
    //Mark time
    m_start = std::chrono::high_resolution_clock::now();
  }
  void FrameRateController::OnFrameEnd()
  {
    //Mark time
    m_end = std::chrono::high_resolution_clock::now();
    //Sleep to meet FPS needs
    FillTimeToMeetFPSLimit();
    //Mark time again, for calculations
    m_end = std::chrono::high_resolution_clock::now();
    //record this frame
    AddToFramesList(m_start, m_end);
    //Do calculations and upkeep
    CalculateFrameRate();
  }
  
  void FrameRateController::FillTimeToMeetFPSLimit()
  {
    //get how much longer we need to wait, till we've filled the time for this frame
    long long timeMilli = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
    long long diffMilli = m_targetDT_InMilli - timeMilli;
    if(diffMilli > 0)
      Sleep((unsigned long)diffMilli);
  }
  
  void FrameRateController::AddToFramesList(Time start, Time end)
  {
    //get how long this frame lasted
    long long timeMilli = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  
    //add it to the list
    m_previousFrames[m_frameIterator++] = timeMilli;
    //update dt
    m_dt = (float)TO_SECONDS(timeMilli);
  
    //fix the frame tracking iterator to wrap properly
    if(m_frameIterator >= FRAMES_TO_TRACK)
    {
      m_frameIterator = 0;
      m_pastTrackedAmount = true;
    }
  }
  
  double FrameRateController::GetAverageFrameDuration(void)
  {
    double avg = 0.0;
    int count = 0;
  
    //get how many frames we've tracked
    if(m_pastTrackedAmount)
      count = FRAMES_TO_TRACK;
    else
      count = m_frameIterator;
  
    //add em up
    for(int i = 0; i < count; ++i)
      avg += m_previousFrames[i];
  
    //average them, this is the FPS we've been running at
    avg = count / TO_SECONDS(avg);
    return avg;
  }
  
  void FrameRateController::CalculateFrameRate(void)
  {
    //get the FPS
    double exactFPS = GetAverageFrameDuration();
    //quality check
    if(exactFPS < 0.0)
      exactFPS = 0.0;
    //update fps
    m_fps = (unsigned)exactFPS;
  }
}