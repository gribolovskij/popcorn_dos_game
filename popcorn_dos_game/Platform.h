#pragma once

#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Redraw_Platform(HWND Hwnd);
	void Draw(HDC hdc, HPEN &bg_pen, HBRUSH &bg_brush, RECT& paint_area);

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