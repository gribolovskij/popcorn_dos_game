#pragma once

#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EPlatform_State
{
	EPS_Missing,
	EPS_Normal,
	EPS_Meltdown,
	EPS_Roll_In,
	EPS_Exp_Roll_In
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Act(HWND Hwnd);
	void Set_State(EPlatform_State new_state);
	void Redraw_Platform(HWND Hwnd);
	void Draw(HDC hdc, RECT& paint_area);
	void Draw_Highlight(HDC hdc, int x, int y);
	void Clear_BG(HDC hdc);


	int Roll_Step;
	int X_Pos;
	int Width;
	int Inner_Width;

	static int Rand(int range);

	static const int Width_Normal = 115;

	int Meltdown_Platform_Y_Pos[Width_Normal];

	static const int X_Step = 20;

private:
	void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
	void Draw_Normal_State(HDC hdc, RECT& paint_area);
	void Draw_Roll_State(HDC hdc, RECT& paint_area);


	EPlatform_State Platform_State;

	RECT Platform_Rect, Prev_Platform_Rect;

	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush;

	static const int Height = 25;
	static const int Meltdown_Speed = 3;
	static const int Max_Roll_Step = 8;
	static const int End_Roll_X_Pos = 450;
	static const int Roll_Platform_Speed = 3;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------