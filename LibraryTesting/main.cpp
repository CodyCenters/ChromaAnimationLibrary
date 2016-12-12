/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#include "..\ChromaAnimationLibrary\ChromaAnimationLibrary.h"
#include "AnimationCreation.h"
#include <cstdio>



using namespace ChromaAnimation;
bool TestAnimationsEqual(Animation* lhs, Animation* rhs)
{
  if(lhs->m_count != rhs->m_count)
    return false;
  if(lhs->m_frameSize != rhs->m_frameSize)
    return false;
  for(unsigned i = 0; i < lhs->m_count; ++i)
    for(unsigned j = 0; j < lhs->m_frameSize; ++j)
      if(lhs->m_animation[i][j] != rhs->m_animation[i][j])
        return false;
  return true;
}

Animation* TestMakeAnimation(void)
{
  Animation* Test = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  for(unsigned i = 0; i < 240; ++i)
    for(unsigned j = 0; j < GetFrameSizeFromDeviceType(DEVICES::KEYBOARD); ++j)
      Test->m_animation[i][j] = RGB(rand()%255, rand()%255, rand()%255);
  return Test;
}

void TestMakeStockFile(void)
{
  FileSaver save;

  Animation* F_GREY = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation* L_GREEN = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation* R_ORANGE = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation* TL_CYAN = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation* BL_PURPLE = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation* TR_YELLOW = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation* BR_BLUE = AnimationCreator::Get().CreateNewAnimation(DEVICES::KEYBOARD, 240);

  for(unsigned i = 0; i < 240; ++i)
  {
    for(unsigned j = 0; j < 6; ++j)
    {
      for(unsigned k = 0; k < 22; ++k)
      {
        //TOP LEFT
        if(j < 3 && k < 11)
        {
          F_GREY->m_animation[i][(j*22) + k] = GREY;
          L_GREEN->m_animation[i][(j*22) + k] = GREEN;
          R_ORANGE->m_animation[i][(j*22) + k] = BLACK;
          TL_CYAN->m_animation[i][(j*22) + k] = CYAN;
          BL_PURPLE->m_animation[i][(j*22) + k] = BLACK;
          TR_YELLOW->m_animation[i][(j*22) + k] = BLACK;
          BR_BLUE->m_animation[i][(j*22) + k] = BLACK;
        }
        else if(j >= 3 && k < 11)//BOTTOM LEFT
        {
          F_GREY->m_animation[i][(j*22) + k] = GREY;
          L_GREEN->m_animation[i][(j*22) + k] = GREEN;
          R_ORANGE->m_animation[i][(j*22) + k] = BLACK;
          TL_CYAN->m_animation[i][(j*22) + k] = BLACK;
          BL_PURPLE->m_animation[i][(j*22) + k] = PURPLE;
          TR_YELLOW->m_animation[i][(j*22) + k] = BLACK;
          BR_BLUE->m_animation[i][(j*22) + k] = BLACK;
        }
        else if(j < 3 && k >= 11)//TOP RIGHT
        {
          F_GREY->m_animation[i][(j*22) + k] = GREY;
          L_GREEN->m_animation[i][(j*22) + k] = BLACK;
          R_ORANGE->m_animation[i][(j*22) + k] = ORANGE;
          TL_CYAN->m_animation[i][(j*22) + k] = BLACK;
          BL_PURPLE->m_animation[i][(j*22) + k] = BLACK;
          TR_YELLOW->m_animation[i][(j*22) + k] = YELLOW;
          BR_BLUE->m_animation[i][(j*22) + k] = BLACK;
        }
        else if(j >= 3 && k >= 11)//BOTTOM RIGHT
        {
          F_GREY->m_animation[i][(j*22) + k] = GREY;
          L_GREEN->m_animation[i][(j*22) + k] = BLACK;
          R_ORANGE->m_animation[i][(j*22) + k] = ORANGE;
          TL_CYAN->m_animation[i][(j*22) + k] = BLACK;
          BL_PURPLE->m_animation[i][(j*22) + k] = BLACK;
          TR_YELLOW->m_animation[i][(j*22) + k] = BLACK;
          BR_BLUE->m_animation[i][(j*22) + k] = BLUE;
        }
      }
    }
  }

  F_GREY->m_layer = F_GREY->m_outerLayer = 0;
  L_GREEN->m_layer = L_GREEN->m_outerLayer = 1;
  R_ORANGE->m_layer = R_ORANGE->m_outerLayer = 1;
  TL_CYAN->m_layer = TL_CYAN->m_outerLayer = 2;
  BL_PURPLE->m_layer = BL_PURPLE->m_outerLayer = 2;
  TR_YELLOW->m_layer = TR_YELLOW->m_outerLayer = 2;
  BR_BLUE->m_layer = BR_BLUE->m_outerLayer = 2;

  save.Save("FullGrey", F_GREY);
  save.Save("LeftGreen", L_GREEN);
  save.Save("RightOrange", R_ORANGE);
  save.Save("TopLeftCyan", TL_CYAN);
  save.Save("BottomLeftPurple", BL_PURPLE);
  save.Save("TopRightYellow", TR_YELLOW);
  save.Save("BottomRightBlue", BR_BLUE);
}

void TestMakingFile(void)
{
  Animation* Test = TestMakeAnimation();
  FileSaver("CodyTest", Test);
}

void TestLoadFile(void)
{
  Animation* TestControl = TestMakeAnimation();
  FileSaver("LoadTest", TestControl);
  FileLoader loader;
  Animation* Test = loader.Load("LoadTest");
  bool res = TestAnimationsEqual(TestControl, Test);
}

void TestFrameRateController(void)
{
  FrameRateController frc(30);

  while(1)
  {
    frc.OnFrameStart();

    printf("FPS: %d, DT: %f\n", frc.GetFPS(), frc.GetDT());
    for(int i = 0; i < 1; ++i)
    {
      Animation* test = TestMakeAnimation();
      AnimationCreator::Get().DeleteAnimation(test);
    }

    frc.OnFrameEnd();
  }
}

void TestPlayAnimation(void)
{
  Animation* Test = TestMakeAnimation();
  FileSaver("Test", Test);

  gSDK.GetAnimationManager().LoadAnimation("Test");
  gSDK.GetAnimationManager().PlayAnimation("Test");
  gSDK.Run();
}

void TestAnimationSort(void)
{
  AnimationCreator& creator = AnimationCreator::Get();
  Animation *o0l0 = creator.CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation *o1l0 = creator.CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation *o1l0_1 = creator.CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation *o1l1 = creator.CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation *o1l2 = creator.CreateNewAnimation(DEVICES::KEYBOARD, 240);
  Animation *o3l4 = creator.CreateNewAnimation(DEVICES::KEYBOARD, 240);

  o0l0->m_outerLayer = 0;
  o0l0->m_layer = 0;

  o1l0->m_outerLayer = 1;
  o1l0->m_layer = 0;

  o1l0_1->m_outerLayer = 1;
  o1l0_1->m_layer = 0;

  o1l1->m_outerLayer = 1;
  o1l1->m_layer = 1;

  o1l2->m_outerLayer = 1;
  o1l2->m_layer = 2;

  o3l4->m_outerLayer = 3;
  o3l4->m_layer = 4;

  FileSaver save;
  save.Save("o0l0", o0l0);
  save.Save("o1l0", o1l0);
  save.Save("o1l0_1", o1l0_1);
  save.Save("o1l1", o1l1);
  save.Save("o1l2", o1l2);
  save.Save("o3l4", o3l4);

  AnimationManager& man = gSDK.GetAnimationManager();
  man.LoadAnimation("o0l0");
  man.LoadAnimation("o1l0");
  man.LoadAnimation("o1l0_1");
  man.LoadAnimation("o1l1");
  man.LoadAnimation("o1l2");
  man.LoadAnimation("o3l4");

  
  man.PlayAnimation("o1l0_1");
  man.PlayAnimation("o1l1");
  man.PlayAnimation("o0l0");
  man.PlayAnimation("o1l0");
  man.PlayAnimation("o3l4");
  man.PlayAnimation("o1l2");
}

void TestLayeredAnimation(void)
{
  gSDK.GetAnimationManager().LoadAnimation("TopLeftCyan");
  gSDK.GetAnimationManager().LoadAnimation("CodyTest");
  gSDK.GetAnimationManager().LoadAnimation("RightOrange");
  gSDK.GetAnimationManager().PlayAnimation("TopLeftCyan");
  gSDK.GetAnimationManager().PlayAnimation("CodyTest");
  gSDK.GetAnimationManager().PlayAnimation("RightOrange");
  gSDK.Run();
}

void TestBreath(void)
{
  gSDK.DoBreath();
}

void TestNewAnimationSystem(void)
{
  gSDK.Run();
  getchar();
  gSDK.GetAnimationManager().LoadAnimation("LeftGreen");
  gSDK.GetAnimationManager().PlayAnimation("LeftGreen");
  getchar();
  gSDK.GetAnimationManager().LoadAnimation("BottomRightBlue");
  gSDK.GetAnimationManager().PlayAnimation("BottomRightBlue");
  getchar();
  gSDK.GetAnimationManager().LoadAnimation("CodyTest");
  gSDK.GetAnimationManager().PlayAnimation("CodyTest");
}

void TestMakeStaticAnimation(void)
{
  Animation* Test = TestMakeAnimation();
  Test->m_static = true;
  FileSaver("CodyTest", Test);
  gSDK.GetAnimationManager().LoadAnimation("CodyTest");
  gSDK.GetAnimationManager().PlayAnimation("CodyTest");
  gSDK.Run();
}

void TestMakeLoopingAnimation(void)
{
  Animation* Test = TestMakeAnimation();
  Test->m_loop = true;
  FileSaver("CodyTest", Test);
  gSDK.GetAnimationManager().LoadAnimation("CodyTest");
  gSDK.GetAnimationManager().PlayAnimation("CodyTest");
  gSDK.Run();
}

int main(void)
{
  if(!gSDK.Initialize())
    return 0;

  //TestMakeAnimation();
  //TestMakingFile();
  //TestLoadFile();
  //TestFrameRateController();
  //TestPlayAnimation();
  //TestMakeStockFile();
  //TestAnimationSort();
  //TestLayeredAnimation();
  //TestBreath();
  //TestNewAnimationSystem();
  //TestMakeStaticAnimation();
  TestMakeLoopingAnimation();

  getchar();
  return 0;
}