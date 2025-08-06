#pragma once

#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EPlatform_State
{
	EPS_Missing,
	EPS_Ready,
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
	void Act();
	EPlatform_State Get_State();
	void Set_State(EPlatform_State new_state);
	void Redraw_Platform();
	void Draw(HDC hdc, RECT& paint_area);

	int Roll_Step;
	int X_Pos;
	int Width;
	int Inner_Width;

	static int Rand(int range);

	static const int Width_Normal = 115;
	static const int X_Step = 20;

	int Meltdown_Platform_Y_Pos[Width_Normal];

private:
	void Clear_BG(HDC hdc);
	void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
	void Draw_Normal_State(HDC hdc, RECT& paint_area);
	void Draw_Roll_State(HDC hdc, RECT& paint_area);
	void Draw_Exp_Roll_State(HDC hdc, RECT &paint_area);
	void Draw_Highlight(HDC hdc, int x, int y);

	EPlatform_State Platform_State;

	RECT Platform_Rect, Prev_Platform_Rect;

	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush;

	static const int Height = 25;
	static const int Meltdown_Speed = 3;
	static const int Max_Roll_Step = 16;
	static const int End_Roll_X_Pos = 390;
	static const int Roll_Platform_Speed = 7;
	static const int Normal_Inner_Width = 40;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------