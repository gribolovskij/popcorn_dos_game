#pragma once

#include <Windows.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public: 
	static void Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN& pen, HBRUSH& brush);

	static const int Global_Scale = 1;
	static const int Circle_Size = 20;
	static const int Volume_Rectangle = 59;
	static const int Y_Letter = 8;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------