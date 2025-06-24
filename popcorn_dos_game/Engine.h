#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
	EKT_Left,
	EKT_Right,
	EKT_Space,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
	ELT_None,

	ELT_O,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
	void Init_Level();
	void Check_Hit_Brick(int& next_y_pos);
	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Draw_Level(HDC hdc);

	HPEN Purple_Brick_Pen, Blue_Brick_Pen, Letter_Pen;
	HBRUSH Purple_Brick_Brush, Blue_Brick_Brush;
	RECT Level_Rect, paint_area;

	static const int Level_Width = 12;		// Width brick
	static const int Level_Height = 14;		// Height brick
	static const int Level_X_Offset = 23;		// Position start level
	static const int Level_Y_Offset = 13;		// Position start level
	static const int Cell_Width = 64;
	static const int Cell_Height = 26;
	static const int Brick_Width = 61;
	static const int Brick_Height = 23;

};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class ABall
{
public:
	ABall();

	void Draw(HDC hdc, RECT& paint_area, AsEngine *engine);
	void Move(AsEngine *engine, ALevel *level);


	double Ball_Direction;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	RECT Ball_Rect, Prev_Ball_Rect;

private:
	int Ball_X_Pos, Ball_Y_Pos;
	int Ball_X_Offset, Ball_Y_Offset;
	double Ball_Speed;

	static const int Ball_Size = 14;

};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ALevel;
class AsEngine
{
public:
	AsEngine();
	
	ABall Ball;
	ALevel level;

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();
	static void Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN& pen, HBRUSH& brush);

	int Platform_X_Pos;
	int Platform_Width;

	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;


	static const int Max_X_Pos = (ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width);
	static const int Max_Y_Pos = 632;
	static const int Platform_Y_Pos = 550;
	static const int Border_X_Offset = 22;
	static const int Border_Y_Offset = 17;
	static const int Global_Scale = 1;
	static const int Volume_Rectangle = 59;
	static const int Y_Letter = 8;
	static const int Circle_Size = 20;

private:
	void Redraw_Platform();
	void Draw_Platform(HDC hdc, int x, int y);
	void Draw_Border(HDC hdc, int x, int y, bool top_border);
	void Draw_Bounce(HDC hdc, RECT& paint_area);

	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen,  Border_Blue_Pen, Border_White_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush,  Border_Blue_Brush, Border_White_Brush;

	int Platform_Inner_Width;

	RECT Platform_Rect, Prev_Platform_Rect;
	
	
	
	static const int Platform_Height = 25;
	static const int Platform_X_Step = 20;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
