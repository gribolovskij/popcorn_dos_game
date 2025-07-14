#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include "Border.h"
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
	void Init();
	void Check_Level_Hit_Brick(int &next_y_pos, double &ball_direction);
	void Draw(HDC hdc);

	static const int Level_Height = 14;		// Height brick
	static const int Level_Width = 12;		// Width brick
	static const int Level_X_Offset = 23;		// Position start level
	static const int Level_Y_Offset = 13;		// Position start level
	static const int Cell_Width = 64;
	static const int Cell_Height = 26;

	RECT Level_Rect;

private:

	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	HPEN Purple_Brick_Pen, Blue_Brick_Pen, Letter_Pen;
	HBRUSH Purple_Brick_Brush, Blue_Brick_Brush;
	RECT paint_area;

	static const int Brick_Width = 61;
	static const int Brick_Height = 23;

};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class AsPlatform;
class ABall
{
public:
	ABall();

	void Init();
	void Draw(HDC hdc, RECT& paint_area, AsEngine *engine);
	void Move(AsEngine *engine, ALevel *level, AsPlatform *platform);


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
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Redraw_Platform(AsEngine *engine);
	void Draw(HDC hdc, AsEngine *engine, RECT& paint_area);

	int X_Pos;
	int Width;

	static const int Circle_Size = 20;
	static const int Y_Pos = 550;
	static const int X_Step = 20;

private:
	int Inner_Width;

	RECT Platform_Rect, Prev_Platform_Rect;

	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush;

	static const int Height = 25;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine

{
public:
	ABall Ball;
	ALevel Level;
	AsPlatform Platform;
	AsBorder Border;

	void Init(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;

	static const int Max_X_Pos = (ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width);
	static const int Max_Y_Pos = 632;
	static const int Global_Scale = 1;
	static const int Volume_Rectangle = 59;
	static const int Y_Letter = 8;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
