#include "Ball.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_Line(double y, double left_x, double right_x, double radius, double next_x_pos, double& x)
{
	// Check intersection horizontal segment (START left_x FINISH right_x) with a circle of radius

	double min_x, max_x;

	if (y > radius)
		return false;

	x = sqrt(radius * radius - y * y);

	max_x = next_x_pos + x;
	min_x = next_x_pos - x;

	if (max_x >= left_x && max_x <= right_x || min_x >= left_x && min_x <= right_x)

		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const double ABall::Start_Ball_Y_Pos = 536.0;
const double ABall::Radius = 7 - 0.5;
int ABall::Count_Hit_Checkers = 0;
AHit_Checker *ABall::Hit_Checkers[] = {};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ABall::ABall()
	: Center_X_Pos(0.0), Center_Y_Pos(Start_Ball_Y_Pos), Ball_Y_Offset(-3), Ball_Speed(0.0), Ball_Direction(0), Ball_Brush(0), Prev_Ball_Rect{}, Ball_Rect{}, Ball_Pen(0),
	Ball_X_Offset(0), Ball_State(EBS_Normal), Rest_Distance(0.0), Test_Iteraton(0), Test_Active(false)
{
	// Set_State(EBS_Normal ,435);
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

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);
	}

	if (Ball_State == EBS_Missing)
		return;
	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))		// Cheking The Field Coloring After The Ball
	{
	//	2. Draw ball
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
	}

	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move()
{
	int i;
	bool got_hit;
	double next_x_pos, next_y_pos;
	int platform_y_pos = AsConfig::Platform_Y_Pos;

	if (Ball_State != EBS_Normal)
		return;

		Prev_Ball_Rect = Ball_Rect;
		Rest_Distance += Ball_Speed;

	while(Rest_Distance >= AsConfig::Step_Move)
	{
		got_hit = false;
		next_x_pos = Center_X_Pos + AsConfig::Step_Move * cos(Ball_Direction);
		next_y_pos = Center_Y_Pos - AsConfig::Step_Move * sin(Ball_Direction);

		// Correction position when reflecting
		for (i = 0; i < Count_Hit_Checkers; i++)
		{
			got_hit |= Hit_Checkers[i] -> Check_Hit(next_x_pos, next_y_pos, this);
		}
		if(! got_hit)
		{
		//	2. Ball continue move, if did not interact with other objects
		Rest_Distance -= AsConfig::Step_Move;

		Center_X_Pos = next_x_pos;
		Center_Y_Pos = next_y_pos;
		
		if (Test_Active)
		Test_Distance -= AsConfig::Step_Move;
		}
	}
		Redraw_Ball();
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Set_Test()
{
	Test_Active = true;
	Test_Distance = 250.0;

	Set_State(EBS_Normal, 320 + Test_Iteraton, 100);
	Ball_Direction = 2.0 * M_PI - M_PI_4;

	++Test_Iteraton;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ABall::Test_Finish()
{
	if (Test_Active)
	{
		if (Test_Distance <= 0)
		{
			Test_Active = false;
			Set_State(EBS_Missing, 0);
			return true;
		}
			}
			return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
	return Ball_State;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, int x_pos, double y_pos)
{
	switch (new_state)
	{
	case EBS_Normal:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 8.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI_4;
		Redraw_Ball();
		break;

	case EBS_Missing:
		Ball_Speed = 0.0;

		break;

	case EBS_Ready:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI_4;
		Redraw_Ball();
		break;
	}
	Ball_State = new_state;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
double ABall::Get_Direction()
{
	return Ball_Direction;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Set_Direction(double new_direction)
{
	const double pi_2 = 2.0 * M_PI;

	while (new_direction > pi_2)
			new_direction -= pi_2;

	while (new_direction < 0.0)
			new_direction += pi_2;

	Ball_Direction = new_direction;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Reflect(bool from_horizontal)
{
	if (from_horizontal)
		Set_Direction(- Ball_Direction);

	else 
		Set_Direction(M_PI - Ball_Direction);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Left()
{
	if (Ball_Direction > M_PI_2 && Ball_Direction < M_PI + M_PI_2)
		return true;
	else
		return false;

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ABall::Is_Moving_Up()
{
	if (Ball_Direction >= 0.0 && Ball_Direction < M_PI)
		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Add_Hit_Checkers(AHit_Checker *hit_checker)
{
	if (Count_Hit_Checkers >= sizeof(Hit_Checkers) / sizeof(Hit_Checkers[0]) )
	return;

	Hit_Checkers[Count_Hit_Checkers++] = hit_checker;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left   =(int) (Center_X_Pos - Radius);
	Ball_Rect.top    =(int) (Center_Y_Pos - Radius);
	Ball_Rect.right  =(int) (Center_X_Pos + Radius);
	Ball_Rect.bottom =(int) (Center_Y_Pos + Radius);

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
