/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "RunningAnimation.h"
#include "AnimationFileLoader.h"
#include "AnimationFileSaver.h"


namespace ChromaAnimation
{
  struct Animation;
  struct LayeredAnimation;
  class CChromaSDKImpl;
  
  class AnimationManager
  {
    friend class FileLoader;
    friend class CChromaSDKImpl;
  public:
    AnimationManager();
    ~AnimationManager();
  
    //Loads animations from files and builds them for use
    bool LoadAnimation(const char* file);
    //Loads a set of animations from files and builds them for use
    bool LoadLayeredAnimation(const char* file);
  
    //Retrieves and animation, if it has been loaded
    Animation* GetAnimation(std::string name);
    //Retrieves the names of the animations in a layered animation
    std::vector<std::string> GetLayeredAnimation(std::string name);
  
    //Begins playing the given animation
    bool PlayAnimation(std::string name);
    //Begins playing the given layered animation
    bool PlayLayeredAnimation(std::string name);
    
    //Sets the FPS to run animations at
    void SetFPS(unsigned fps){m_FPS = fps;}
    //Gets the FPS animations run at
    unsigned GetFPS(void){return m_FPS;}
  
    //Get which animations are currently being played
    std::vector<RunningAnimation>& GetRunningAnimationsOnDevice(unsigned device);
  
    //Remove all completed animations from the list of currently playing animations
    void RemoveCompletedAnimations(void);
  
  private:
    //animations currently running
    std::unordered_map<unsigned, std::vector<RunningAnimation> > m_runningAnimations;
    //animations that are loaded and ready to use.
    std::unordered_map<std::string, Animation*> m_loadedAnimations;
    //arrays of animations to be played at once
    std::unordered_map<std::string, std::vector<std::string> > m_layeredAnimations;
  
    //[NEW] Creates a new Animation
    Animation* CreateNewAnimation(std::string name, unsigned device, unsigned long framecount);
    //[DELETE] Destroys an animation
    void DeleteAnimation(std::string name);
    //Pulls an auto load file of animations to load on startup
    void CheckAutoLoad(void);
    //Destroys all animations
    void Cleanup(void);
    //Sort animations for a device, based on layer
    void SortAnimations(unsigned devicetype);
  
    unsigned m_FPS;
    FileLoader m_fileLoader;
    FileSaver m_fileSaver;
  };
}