#pragma once

#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EPlatform_State
{
	EPS_Missing,
	EPS_Normal,
	EPS_Meltdown
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Act(HWND Hwnd);
	void Redraw_Platform(HWND Hwnd);
	void Draw(HDC hdc, RECT& paint_area);
	static int Rand(int range);

	int X_Pos;
	int Width;
	int Inner_Width;
	static const int Width_Normal = 115;

	int Meltdown_Platform_Y_Pos[Width_Normal];


	static const int X_Step = 20;

private:
	void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
	void Draw_Normal_State(HDC hdc, RECT& paint_area);

	EPlatform_State Platform_State;

	RECT Platform_Rect, Prev_Platform_Rect;

	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush;

	static const int Height = 25;
	static const int Meltdown_Speed = 3;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------