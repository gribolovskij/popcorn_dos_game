#include "Ball.h"

// ABall
const double ABall::Start_Ball_Y_Pos = 536.0;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Ball_X_Pos(0.0), Ball_Y_Pos(Start_Ball_Y_Pos), Ball_Y_Offset(-3), Ball_Speed(0.0), Ball_Direction(0), Ball_Brush(0), Prev_Ball_Rect{}, Ball_Rect{}, Ball_Pen(0), Ball_X_Offset(0),
	Ball_State(EBS_Normal)
{
	Set_State(EBS_Ready ,435);
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

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))			// Cheking The Field Coloring After The Ball
	{
	//	1. Clean background
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
}

	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))			// Cheking The Field Coloring After The Ball
	{
		//	2. Draw ball
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move(ALevel *level, int platform_x_pos, int platform_width)
{
	double next_x_pos, next_y_pos;
	int max_x_pos = AsConfig::Max_X_Pos - Ball_Size;
	int platform_y_pos = AsConfig::Platform_Y_Pos - Ball_Size;
	int max_y_pos = AsConfig::Max_Y_Pos - Ball_Size;	

	if (Ball_State != EBS_Normal)
		return;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + (Ball_Speed * cos(Ball_Direction) );
	next_y_pos = Ball_Y_Pos - (Ball_Speed * sin(Ball_Direction) );

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
	if (next_y_pos > max_y_pos)
	{
		if (level->Has_Floor)
		{
			next_y_pos = max_y_pos - (next_y_pos - max_y_pos);				//bottom
			Ball_Direction = - Ball_Direction;
		}
		else
		{
			if (next_y_pos < (double)max_y_pos + (double)Ball_Size)

			Ball_State = EBS_Missing;
		}		
}

	// Correction position when reflecting from the platform
	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
		{
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
			Ball_Direction = -Ball_Direction;
		}
	}

	// Correction position when reflecting from the bricks
	level->Check_Level_Hit_Brick(next_y_pos, Ball_Direction);

	//	2. Move the ball
	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Ball_Rect.left = (int)Ball_X_Pos;
	Ball_Rect.top = (int)Ball_Y_Pos;
	Ball_Rect.right = Ball_Rect.left + Ball_Size;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size;

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
	return Ball_State;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, int x_pos)
{
	switch (new_state)
	{
	case EBS_Normal:
		Ball_X_Pos = (double)x_pos - (double)Ball_Size / 2.0;
		Ball_Y_Pos = Start_Ball_Y_Pos;
		Ball_State = EBS_Normal;
		Ball_Speed = 8.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;

	case EBS_Missing:
		Ball_Speed = 0.0;

		break;

	case EBS_Ready:
		Ball_X_Pos = (double)x_pos - (double)Ball_Size / 2.0;
		Ball_Y_Pos = Start_Ball_Y_Pos;
		Ball_State = EBS_Ready;
		Ball_Speed = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;
	}
	Ball_State = new_state;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)Ball_X_Pos;
	Ball_Rect.top = (int)Ball_Y_Pos;
	Ball_Rect.right = Ball_Rect.left + Ball_Size;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size;

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
