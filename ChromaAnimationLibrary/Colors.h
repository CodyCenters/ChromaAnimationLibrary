/*
All Code in this project, Chroma Animation Library, is written by Cody Centers.
*/
#pragma once


namespace ChromaAnimation
{
  #define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
  #define GetRValue(rgb)      (LOBYTE(rgb))
  #define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
  #define GetBValue(rgb)      (LOBYTE((rgb)>>16))
  
  
  const COLORREF BLACK = RGB(0,0,0);
  const COLORREF WHITE = RGB(255,255,255);
  const COLORREF RED = RGB(255,0,0);
  const COLORREF GREEN = RGB(0,255,0);
  const COLORREF BLUE = RGB(0,0,255);
  const COLORREF YELLOW = RGB(255,255,0);
  const COLORREF PURPLE = RGB(128,0,128);
  const COLORREF CYAN = RGB(00,255,255);
  const COLORREF ORANGE = RGB(255,165,00);
  const COLORREF PINK = RGB(255,192,203);
  const COLORREF GREY = RGB(125, 125, 125);
}