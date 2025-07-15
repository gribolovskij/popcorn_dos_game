#pragma once

#include "Config.h"
#include "Level.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Draw(HDC hdc, RECT& paint_area, HPEN &bg_pen, HBRUSH &bg_brush);
	void Move(HWND Hwnd, ALevel *level, int platform_x_pos, int platform_width);


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