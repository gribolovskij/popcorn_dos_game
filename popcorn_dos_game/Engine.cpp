#include "Engine.h"

// AsEngine
AsEngine::AsEngine()
	: 	Hwnd{}
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init(HWND hwnd)
//	Initialization function |=|=|=| Setting up the game at startup
{
	Hwnd = hwnd;

	AAction_Brick::Setup_Colors();

	Level.Init();
	Ball.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw_Platform(Hwnd);

	SetTimer(Hwnd, Timer_ID, 1000/AsConfig::FPS, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
//	Drawing screen game
{
	Level.Draw(hdc, Hwnd);
	Platform.Draw(hdc, paint_area);
	Ball.Draw(hdc, paint_area);
	Border.Draw(hdc, paint_area);
	/*int i;
	for (i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 200, EBT_Blue, ELT_O, i);
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 130, EBT_Purple, ELT_O, i);
	}*/
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

	Level.Action_Brick.Act(Hwnd);

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
