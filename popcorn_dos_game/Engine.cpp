#include "Engine.h"

// AsEngine
AsEngine::AsEngine()
	: 	Hwnd{}, BG_Pen(0), BG_Brush(0)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init(HWND hwnd)
//	Initialization function |=|=|=| Setting up the game at startup
{
	Hwnd = hwnd;

	AsConfig::Create_Pen_Brush(3, 105, 24, BG_Pen, BG_Brush);

	Level.Init();
	Ball.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw_Platform(Hwnd);

	SetTimer(Hwnd, Timer_ID, 1, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
//	Drawing screen game
{
		Level.Draw(hdc);

	Platform.Draw(hdc, BG_Pen, BG_Brush, paint_area);

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

		Platform.Redraw_Platform(Hwnd);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;
		if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 32)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 32;

		Platform.Redraw_Platform(Hwnd);
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
