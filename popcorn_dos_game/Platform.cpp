#include "Platform.h"


// AsPlatform
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: X_Pos(AsConfig::Border_X_Offset), Width(Width_Normal), Inner_Width(Normal_Inner_Width), Arc_Pen(0), Arc_Brush(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), 
	Platform_State(EPS_Normal), Platform_Circle_Brush(0), Platform_Inner_Brush(0), Platform_Rect{}, Prev_Platform_Rect{}, Meltdown_Platform_Y_Pos{}, Roll_Step(0)
{
	X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
// Correction position when reflecting from the platform
	if (next_y_pos + ball -> Radius > AsConfig::Platform_Y_Pos)
	{
		if (next_x_pos + ball -> Radius >= X_Pos && next_x_pos - ball -> Radius<= X_Pos + Width)
		{
			ball -> Reflect(true);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	AsConfig::Create_Pen_Brush(155, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(249, 100, 0, Platform_Inner_Pen, Platform_Inner_Brush);
	AsConfig::Create_Pen_Brush(255, 255, 255, Arc_Pen, Arc_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Act()
{
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
			Meltdown_Platform_Y_Pos[i] = AsConfig::Platform_Y_Pos + Height;
	break; 


	case EPS_Roll_In:
		X_Pos = AsConfig::Max_X_Pos + 14;
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

Arc	(hdc, x + AsConfig::Level_Y_Offset, y + AsConfig::Level_Y_Offset, x + AsConfig::Ball_X_Offset, y + 2, x + 10, y - AsConfig::Brick_Width, x - 121, y - AsConfig::Brick_Width);
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
	int x = X_Pos ;
	int y = AsConfig::Platform_Y_Pos;

	// Cleen the window, when moving the platform
	Clear_BG(hdc);
	
	// 1. Drawing lateral circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x, y, x + AsConfig::Circle_Size * AsConfig::Global_Scale, y + AsConfig::Circle_Size * AsConfig::Global_Scale);

	Ellipse(hdc, x + (AsConfig::Circle_Size + Inner_Width), y, x + ((AsConfig::Circle_Size*2)+Inner_Width), y + AsConfig::Circle_Size);

	// 2. Drawing inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y + AsConfig::Global_Scale*2, 10 * AsConfig::Global_Scale, 32 * AsConfig::Global_Scale);

	// 3. Drawing highlight
	Draw_Highlight(hdc, x, y);
}		
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
//	Drawing platform for meltdown state
{	int i, j;
	int y_offset;
	int x, y;
	int area_width, area_height;
	int moved_col_count = 0;
	int max_platform_y;
	COLORREF pixel;
	COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

	area_width = Width * AsConfig::Global_Scale;
	area_height = Height * AsConfig::Global_Scale + 1;

	max_platform_y = AsConfig::Max_Y_Pos + area_height;

	for (i = 0; i < area_width; i++)
	{
		if(Meltdown_Platform_Y_Pos[i] > max_platform_y)
			continue;
		++moved_col_count;

		y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
		x = Platform_Rect.left + i;

		for (j = 0; j < area_height; j++)
		{
			y = Meltdown_Platform_Y_Pos[i] - j;

			pixel = GetPixel(hdc, x, y);
			SetPixel(hdc, x, y + y_offset, pixel);
		}

		for (j = 0; j < y_offset; j++)
		{
			y = Meltdown_Platform_Y_Pos[i] - area_height + 1 + j;
			SetPixel(hdc, x, y, bg_pixel);
		}
		Meltdown_Platform_Y_Pos[i] += y_offset;
	}

	if (moved_col_count == 0)
		Platform_State = EPS_Missing;		// The platform has been moved beyond the window border.
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

	Ellipse(hdc, x, y, x + roller_size, y + roller_size);

	// 2. Draw line border ball and rotate 30 degrees counterclockwise.
	SetGraphicsMode(hdc, GM_ADVANCED);

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

	Rectangle(hdc, -3 / 2, -roller_size / 2, 3 / 2 - 1, roller_size / 2); 

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
