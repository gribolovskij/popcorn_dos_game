#include "Engine.h"

// CLASS ABALL
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
void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
		RECT intersection_rect;

		if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))			// Cheking The Field Coloring After The Ball
			return;
	//	1. Clean background
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

	//	2. Draw ball
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
	int next_x_pos, next_y_pos;
	int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
	int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
	next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

	//	1. Correction position when reflecting from the frame
	if (next_x_pos < AsBorder::X_Offset)
	{
		next_x_pos = ALevel::Level_X_Offset - (next_x_pos -ALevel:: Level_X_Offset);	//left
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < AsBorder::Y_Offset)
	{
		next_y_pos = AsBorder::Y_Offset - (next_y_pos - AsBorder::Y_Offset);	//top
		Ball_Direction = - Ball_Direction;
	}
	if (next_x_pos > max_x_pos)
	{
		next_x_pos = max_x_pos - (next_x_pos - max_x_pos);				//right
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos > AsEngine::Max_Y_Pos)
	{
		next_x_pos = AsEngine::Max_X_Pos - (next_x_pos - AsEngine::Max_X_Pos);				//bottom
		Ball_Direction = M_PI_2 - Ball_Direction;
	}
	
	// Correction position when reflecting from the platform
	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform->X_Pos && next_x_pos <= platform->X_Pos + platform->Width)
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

	InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





// AsPlatform
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: X_Pos(AsBorder::X_Offset), Width(115), Inner_Width(40), Arc_Pen(0), Arc_Brush(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0), Platform_Rect{}, Prev_Platform_Rect{}
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	AsConfig::Create_Pen_Brush(155, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(249, 100, 0, Platform_Inner_Pen, Platform_Inner_Brush);
	AsConfig::Create_Pen_Brush(255, 255, 255, Arc_Pen, Arc_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(AsEngine *engine)
{	
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = X_Pos;
	Platform_Rect.top = Y_Pos;
	Platform_Rect.right = Platform_Rect.left + Width;
	Platform_Rect.bottom = Platform_Rect.top + Height;

	InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, AsEngine *engine, RECT& paint_area)
//	Drawing platform
{
	int x = X_Pos;
	int y = Y_Pos;

	RECT intersection_rect;
	
	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	// Cleen the window, when moving the platform
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Drawing lateral circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + Circle_Size * AsEngine::Global_Scale, y + Circle_Size * AsEngine::Global_Scale);
	Ellipse(hdc, x + (Circle_Size + Inner_Width), y, x + ((Circle_Size*2)+Inner_Width), y + Circle_Size);

	// 2. Drawing inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y + AsEngine::Global_Scale*2, 10 * AsEngine::Global_Scale, 32 * AsEngine::Global_Scale);

	// 3. Drawing highlight
	SelectObject(hdc, Arc_Pen);
	Arc	(hdc, x+13, y+13, x+3, y+2, x+10, y - 61, x - 121, y - 61 );
	//	good arc!! very good nice!!!
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






// AsEngine
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init(HWND hwnd)
//	Функция инициализации |=|=|=| Настройка игры при старте
{
	Hwnd = hwnd;

	AsConfig::Create_Pen_Brush(3, 105, 24, BG_Pen, BG_Brush);

	Level.Init();
	Ball.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw_Platform(this);

	SetTimer(Hwnd, Timer_ID, 1, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
//	Drawing screen game
{
		Level.Draw(hdc);

	Platform.Draw(hdc, this, paint_area);

	/*int i;
	for (i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 200, EBT_Blue, ELT_O, i);
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 130, EBT_Purple, ELT_O, i);
	}*/
	Ball.Draw(hdc, paint_area, this);

	Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	// Platform movement from pressing WINDOW_Keys_Virtual
	switch (key_type)
	{
	case EKT_Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos <= Border.X_Offset)
			Platform.X_Pos = Border.X_Offset;

		Platform.Redraw_Platform(this);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;
		if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 32)
			Platform.X_Pos = Max_X_Pos - Platform.Width + 32;

		Platform.Redraw_Platform(this);
		break;

	case EKT_Space:
		break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	Ball.Move(this, &Level, &Platform);

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
