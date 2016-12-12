/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once
#include <chrono>
#include <vector>

namespace ChromaAnimation
{
  typedef std::chrono::high_resolution_clock::time_point Time;
  
  
  class FrameRateController
  {
  public:
    FrameRateController(unsigned fps);
    ~FrameRateController(){}
  
    //Retrieves the target FPS to run at
    unsigned GetTargetFPS(void){return m_targetFPS;}
    //Sets the target FPS to run at
    void SetTargetFPS(unsigned fps){m_targetFPS = fps;}
    //Retrieves the current FPS
    unsigned GetFPS(void){return m_fps;}
    //Retrieves the dt of the last frame
    float GetDT(void){return m_dt;}
  
    //Required function to be called at the start of main loop
    void OnFrameStart();
    //Required function to be called at the end of main loop
    void OnFrameEnd();
  
  
  private:
    float m_dt;
    unsigned m_fps;
  
    long long m_targetDT_InMilli;
    unsigned m_targetFPS;
  
    //Sleeps an amount of time to meet needed FPS
    void FillTimeToMeetFPSLimit();
  
    //Gets the FPS of the last amount of FRAMES_TO_TRACK
    double GetAverageFrameDuration(void);
    //Updates the FPS
    void CalculateFrameRate(void);
  
    //Update the last amount of frames we track
    void AddToFramesList(Time start, Time end);
  
    int m_frameIterator;
    bool m_pastTrackedAmount;
  
    Time m_start;
    Time m_end;
    std::vector<long long> m_previousFrames;
  };
}