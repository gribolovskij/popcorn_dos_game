#include "Engine.h"

// AsPlatform
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: X_Pos(AsConfig::X_Offset), Width(115), Inner_Width(40), Arc_Pen(0), Arc_Brush(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0), Platform_Rect{}, Prev_Platform_Rect{}
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
	Platform_Rect.top = AsConfig::Y_Pos;
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
	int y = AsConfig::Y_Pos;

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
	Ellipse(hdc, x, y, x + AsConfig::Circle_Size * AsConfig::Global_Scale, y + AsConfig::Circle_Size * AsConfig::Global_Scale);
	Ellipse(hdc, x + (AsConfig::Circle_Size + Inner_Width), y, x + ((AsConfig::Circle_Size*2)+Inner_Width), y + AsConfig::Circle_Size);

	// 2. Drawing inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y + AsConfig::Global_Scale*2, 10 * AsConfig::Global_Scale, 32 * AsConfig::Global_Scale);

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
	Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);

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

		if (Platform.X_Pos <= AsConfig::X_Offset)
			Platform.X_Pos = AsConfig::X_Offset;

		Platform.Redraw_Platform(this);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;
		if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 32)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 32;

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
	Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
