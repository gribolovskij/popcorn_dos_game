#pragma once

#include <Windows.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
	void Init();
	void Draw(HDC hdc, RECT& paint_area, HPEN &bg_pen, HBRUSH &bg_brush);

	static const int X_Offset = 22;
	static const int Y_Offset = 17;

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN &bg_pen, HBRUSH &bg_brush );

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;
};