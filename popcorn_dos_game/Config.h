#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//AColor
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	AColor(unsigned  char r, unsigned  char g, unsigned  char b);
	
	unsigned char R, G, B;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// AsConfig
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public: 
	static void Setup_Colors();
	static void Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN& pen, HBRUSH& brush);
	static void Create_Pen_Brush(const AColor &color, HPEN& pen, HBRUSH& brush);
	static int Rand(int range);

	static int 	Tick_Current_Timer;

	static const AColor Gray_Brick_Color, Orange_Brick_Color, BG_Color;
	static HPEN BG_Pen;
	static HBRUSH BG_Brush;

	static const int Global_Scale = 1;
	static const int Circle_Size = 20;
	static const int Volume_Rectangle = 59; 
	static const int Y_Letter = 8;
	static const int Level_Height = 14;		// Height brick
	static const int Level_Width = 12;		// Width brick
	static const int Level_X_Offset = 23;		// Position start level
	static const int Level_Y_Offset = 13;		// Position start level
	static const int Brick_Width = 61;
	static const int Brick_Height = 23; 
	static const int Cell_Width = 64;
	static const int Cell_Height = 26;
	static const int Max_X_Pos = (Level_X_Offset + Cell_Width * Level_Width);
	static const int Max_Y_Pos = 632;
	static const int X_Offset = 22;
	static const int Y_Offset = 17;
	static const int Platform_Y_Pos = 550;
	static const int Fault_Variable = 39;
	static const int FPS = 40;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------