#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"

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
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Redraw_Platform(AsEngine *engine);
	void Draw(HDC hdc, AsEngine *engine, RECT& paint_area);

	int X_Pos;
	int Width;
	int Inner_Width;

	static const int X_Step = 20;

private:

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
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
