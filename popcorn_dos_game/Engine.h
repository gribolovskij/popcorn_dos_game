#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
	EKT_Left,
	EKT_Right,
	EKT_Space,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
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
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
