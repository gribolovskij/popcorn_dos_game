#include "Platform.h"

// AsPlatform
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::~AsPlatform()
{
	delete[] Image_Normal_Platform;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: X_Pos(AsConfig::Border_X_Offset), Width(Width_Normal), Inner_Width(Normal_Inner_Width), Arc_Pen(0), Arc_Brush(0), Image_Normal_Width(0), Image_Normal_Height(0), Image_Normal_Platform(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), 
	Platform_State(EPS_Normal), Platform_Circle_Brush(0), Platform_Inner_Brush(0), Platform_Rect{}, Prev_Platform_Rect{}, Meltdown_Platform_Y_Pos{}, Roll_Step(0),
	Platform_Circle_Pen_Color(155, 0, 0), Platform_Inner_Pen_Color(249, 100, 0), Arc_Pen_Color(255, 255, 255)
{
	X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	double reflect_pos;
	double inner_left_x, inner_right_x; 
	double inner_top_y, inner_low_y;
	double inner_y;

// Correction position when reflecting from the platform
	if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos)
		return false;

	inner_top_y = (double)(AsConfig::Platform_Y_Pos - ball->Radius);
	inner_low_y = (double)(AsConfig::Platform_Y_Pos + Height - ball->Radius);
	inner_left_x = (double)(X_Pos + ball->Ball_Size);
	inner_right_x = (double)(X_Pos + Width - ball->Ball_Size);

// Check reflect left circle platform
	if (Reflect_Platform_Circle(next_x_pos, next_y_pos, 0.0, ball))
		return true;
	else
		(Reflect_Platform_Circle(next_x_pos, next_y_pos, Width - AsConfig::Circle_Size, ball));

// Check hit central platform up && down
	if (ball->Is_Moving_Up())
		inner_y = inner_low_y;

	else
		inner_y = inner_top_y;

		if (Hit_Circle_Line(next_y_pos - inner_y, inner_left_x, inner_right_x, ball->Radius, next_x_pos, reflect_pos))
		{
			ball->Reflect(true);
			return true;
		}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	AsConfig::Create_Pen_Brush(Platform_Circle_Pen_Color, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(Platform_Inner_Pen_Color, Platform_Inner_Pen, Platform_Inner_Brush);
	AsConfig::Create_Pen_Brush(Arc_Pen_Color, Arc_Pen, Arc_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Act()
{
/*
	if (AsConfig::Tick_Current_Timer % 10 != 0)
		return; TEST ANIMATIONS
*/
	switch (Platform_State)
	{
		case EPS_Meltdown:
		case EPS_Roll_In:
		case EPS_Exp_Roll_In:

		Redraw_Platform();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State()
{
	return Platform_State;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
	int i, len;

	if (Platform_State == new_state)
		return;

	switch (new_state)
	{
	case EPS_Meltdown:
				len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

		for (i = 0; i < len; i++)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
	break;


	case EPS_Roll_In:
		X_Pos = AsConfig::Max_X_Pos;
		Roll_Step = Max_Roll_Step - 1;
		break;
	}
		Platform_State = new_state;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform()
{
	int platform_width;
	Prev_Platform_Rect = Platform_Rect;

	//if (Platform_State == EPS_Normal)

	//	Platform_Rect.left = X_Pos - AsConfig::Centering_Level;

	if (Platform_State == EPS_Roll_In)
		platform_width = AsConfig::Circle_Size;
	
	else
		platform_width = Width;

	Platform_Rect.left = X_Pos; 
	Platform_Rect.top = AsConfig::Platform_Y_Pos;
	Platform_Rect.right = Platform_Rect.left + platform_width;
	Platform_Rect.bottom = Platform_Rect.top + Height;

	if (Platform_State == EPS_Meltdown)
	{
		Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 2);
		Prev_Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 2);
	}

	InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
//	Drawing platform for normal state
{
	RECT intersection_rect;

	// EXAMINATION
	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	switch (Platform_State)
	{
		case EPS_Normal:
		Draw_Normal_State(hdc, paint_area);
	break;

		case EPS_Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
	break;

		case EPS_Roll_In:
		Draw_Roll_State(hdc, paint_area);
	break;

		case EPS_Exp_Roll_In:
		Draw_Exp_Roll_State(hdc, paint_area);
	break;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Highlight(HDC hdc, int x, int y)
{
SelectObject(hdc, Arc_Pen);
Arc	(hdc, x + AsConfig::Level_Y_Offset, y + AsConfig::Level_Y_Offset, x + AsConfig::Ball_X_Offset - 1, y + 2 - 1, x + 10, y - AsConfig::Brick_Width, x - 121, y - AsConfig::Brick_Width);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc)
{
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
//	Drawing platform for normal state
{
	int i, j;
	int offset = 0;

	int x = X_Pos ;
	int y = AsConfig::Platform_Y_Pos;

	// Cleen the window, when moving the platform
	Clear_BG(hdc);
	
	// 1. Drawing lateral circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x, y, x + AsConfig::Circle_Size - 1, y + AsConfig::Circle_Size - 1);

	Ellipse(hdc, x + (AsConfig::Circle_Size + Inner_Width), y, x + ((AsConfig::Circle_Size*2)+Inner_Width) - 1, y + AsConfig::Circle_Size - 1);

	// 2. Drawing inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y + 2, 10, 32);

	// 3. Drawing highlight
	Draw_Highlight(hdc, x, y);

	x *= AsConfig::Global_Scale;
	y *= AsConfig::Global_Scale;

	if (Image_Normal_Platform == 0)
	{
		Image_Normal_Width = Width * AsConfig::Global_Scale;
		Image_Normal_Height = Height * AsConfig::Global_Scale;

		Image_Normal_Platform= new int [Image_Normal_Width * Image_Normal_Height];
		for (i = 0; i < Image_Normal_Height; i++)
			for (j = 0; j < Image_Normal_Width; j++)

				Image_Normal_Platform[offset++] = GetPixel(hdc, x + j, y + i);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
//	Drawing platform for meltdown state
{	int i, j;
	int y_offset;

	int x, y;
	int moved_col_count = 0;
	int max_platform_y;
	int stroke_len;

	HPEN color_pen;
	COLORREF pixel;
	COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

	max_platform_y = AsConfig::Max_Y_Pos + 3; 
	
	for (i = 0; i < Image_Normal_Width; i++)
	{
		if(Meltdown_Platform_Y_Pos[i] > max_platform_y)
			continue;
		++moved_col_count;

		y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
		x = Platform_Rect.left + i;
		
	j = 0;
	y = Meltdown_Platform_Y_Pos[i];

	MoveToEx(hdc, x, y, 0);

	// Drawing subsequence vertical strokes different colors.
	while (Get_Platform_Image_Stroke_Color(i, j, color_pen, stroke_len) )

		{
			SelectObject(hdc, color_pen);

			LineTo(hdc, x, y + stroke_len);

			y += stroke_len;
			j += stroke_len;
		}

	// Clean BG after Meltdown_Platform

	y = Meltdown_Platform_Y_Pos[i];
	MoveToEx(hdc, x, y, 0);
	SelectObject(hdc, AsConfig::BG_Pen);
	LineTo(hdc, x, y + y_offset);
	Meltdown_Platform_Y_Pos[i] += y_offset;

	}

	if (moved_col_count == 0)
		// The platform has been moved beyond the window border.
		Platform_State = EPS_Missing;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_State(HDC hdc, RECT& paint_area)
{
	int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;
	double alpha;
	int roller_size = AsConfig::Circle_Size;
	XFORM xForm, old_xForm;

	Clear_BG(hdc);
	
	// 1. Draw Ball
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x, y, x + roller_size - 1, y + roller_size - 1);

	// 2. Draw line border ball and rotate 30 degrees counterclockwise.

	alpha = -2.0 * M_PI / (double)Max_Roll_Step * (double)Roll_Step;

	xForm.eM11 = (float)cos(alpha);
	xForm.eM12 = (float)sin(alpha);
	xForm.eM21 = (float)-sin(alpha);
	xForm.eM22 = (float)cos(alpha);
	xForm.eDx = (float)(x + roller_size / 2);
	xForm.eDy = (float)(y + roller_size / 2);

	GetWorldTransform(hdc, &old_xForm);
	SetWorldTransform(hdc, &xForm);

	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	Rectangle(hdc, -3 / 2, -roller_size / 2, 3 / 2 - 1 - 1, roller_size / 2 - 1);

	SetWorldTransform(hdc, &old_xForm);

	// 3.
	Draw_Highlight(hdc, x, y);

	++Roll_Step;

	if (Roll_Step >= Max_Roll_Step)
		Roll_Step -= Max_Roll_Step;

	X_Pos -= Roll_Platform_Speed;

	if (X_Pos <= End_Roll_X_Pos)
	{
		X_Pos += Roll_Platform_Speed;
		Platform_State = EPS_Exp_Roll_In;
		Inner_Width = 1;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Exp_Roll_State(HDC hdc, RECT &paint_area)
{
	// Draw expanding platform after rolling out

	Draw_Normal_State(hdc, paint_area);

	--X_Pos;

	Inner_Width += 1;

	if(Inner_Width >= Normal_Inner_Width)
	{
		Inner_Width = Normal_Inner_Width;
		Platform_State = EPS_Ready;
		Redraw_Platform();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_Platform_Circle(double next_x_pos, double next_y_pos, double dis_y_offset, ABall *ball)
{	// Check hit platform right and left circle

	double dx, dy, dis_x, dis_y;
	double distance, two_radius;
	double platform_circle_radius;
	double alpha, betta, gamma;
	double releated_ball_direction;

	const double pi_2 = 2.0 * M_PI;

	platform_circle_radius = AsConfig::Circle_Size / 2;

	dis_x = (double)(X_Pos + platform_circle_radius);
	dis_y = (double)(AsConfig::Platform_Y_Pos + platform_circle_radius);

	dx = next_x_pos - dis_x;
	dy = next_y_pos - dis_y;

	distance = sqrt(dx * dx + dy * dy);

	two_radius = ball->Radius + platform_circle_radius;

	if (fabs(distance - two_radius) < AsConfig::Step_Move)
	{	// ball hit circle platform
		betta = atan2(-dy, dx);

		releated_ball_direction = ball->Get_Direction();
		releated_ball_direction -= betta;

		if (releated_ball_direction > pi_2)
			releated_ball_direction -= pi_2;

		if(releated_ball_direction < 0.0)
			releated_ball_direction += pi_2;

		if (releated_ball_direction > M_PI_2 && releated_ball_direction < M_PI + M_PI_2)
		{
			alpha = betta + M_PI - ball->Get_Direction();
			gamma = betta + alpha;

			ball->Set_Direction(gamma);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, HPEN &color_pen, int &stroke_len)
{
	int i = 0;
	int offset = y * Image_Normal_Width + x;	// Position in massiv Image_Normal_Platform == offset x, y

	int color;
	stroke_len = 0;


	if (y >= Image_Normal_Height)
		return false;

	for (i = y; i < Image_Normal_Height; i++)
	{
		if (i == y)
		{
			color = Image_Normal_Platform[offset];
			stroke_len = 1;
		}

		else
			if (color == Image_Normal_Platform[offset])
				++stroke_len;

			else
				break;

		offset += Image_Normal_Width;
	}

	if (color == Platform_Circle_Pen_Color.Get_RGB() )
		color_pen = Platform_Circle_Pen;

	else if (color == Platform_Inner_Pen_Color.Get_RGB() )
		color_pen = Platform_Inner_Pen;

	else if (color == Arc_Pen_Color.Get_RGB() )
		color_pen = Arc_Pen;

	else if (color == AsConfig::BG_Color.Get_RGB() )
		color_pen = AsConfig::BG_Pen;


	return true;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
