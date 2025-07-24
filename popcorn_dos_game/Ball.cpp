#include "Ball.h"

// ABall
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_X_Pos(64), Ball_Y_Pos(500), Ball_X_Offset(3), Ball_Y_Offset(-3), Ball_Speed(6), Ball_Direction(M_PI - M_PI_4), Ball_Brush(0), Prev_Ball_Rect{}, Ball_Rect{}, Ball_Pen(0)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
	AsConfig::Create_Pen_Brush(255, 0, 0, Ball_Pen, Ball_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))			// Cheking The Field Coloring After The Ball
		return;
	//	1. Clean background
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

	//	2. Draw ball
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move(HWND Hwnd, ALevel *level, int platform_x_pos, int platform_width)
{
	int next_x_pos, next_y_pos;
	int max_x_pos = AsConfig::Max_X_Pos - Ball_Size;
	int platform_y_pos = AsConfig::Platform_Y_Pos - Ball_Size;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
	next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

	//	1. Correction position when reflecting from the frame
	if (next_x_pos < AsConfig::X_Offset)
	{
		next_x_pos = AsConfig::Level_X_Offset - (next_x_pos - AsConfig:: Level_X_Offset);	//left
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < AsConfig::Y_Offset)
	{
		next_y_pos = AsConfig::Y_Offset - (next_y_pos - AsConfig::Y_Offset);	//top
		Ball_Direction = - Ball_Direction;
	}
	if (next_x_pos > max_x_pos)
	{
		next_x_pos = max_x_pos - (next_x_pos - max_x_pos);				//right
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos > AsConfig::Max_Y_Pos)
	{
		next_x_pos = AsConfig::Max_X_Pos - (next_x_pos - AsConfig::Max_X_Pos);				//bottom
		Ball_Direction = M_PI_2 - Ball_Direction;
	}

	// Correction position when reflecting from the platform
	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
		{
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	// Correction position when reflecting from the bricks
	level->Check_Level_Hit_Brick(next_y_pos, Ball_Direction);

	//	2. Move the ball
	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;
	Ball_Rect.left = Ball_X_Pos;
	Ball_Rect.top = Ball_Y_Pos;
	Ball_Rect.right = Ball_Rect.left + Ball_Size;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size;

	InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





