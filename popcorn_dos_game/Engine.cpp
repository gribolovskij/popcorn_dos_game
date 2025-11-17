#include "Engine.h"

// AsEngine
AsEngine::AsEngine()
	: Game_State(EGS_Play_Level)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init(HWND hwnd)
//	Initialization function |=|=|=| Setting up the game at startup
{
	// RANDOM!
	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

	srand(file_time.dwLowDateTime);
	//!
	
	AsConfig::Hwnd = hwnd;

	AAction_Brick::Setup_Colors();

	Level.Init();
	Platform.Init();
	Ball.Init();
	Border.Init();

	ABall::Add_Hit_Checkers(&Level);
	ABall::Add_Hit_Checkers(&Platform);
	ABall::Add_Hit_Checkers(&Border);

	Level.Set_Current_Level(ALevel::Level_01);

	Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);

	Platform.Set_State(EPS_Normal);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000/AsConfig::FPS, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
//	Drawing screen game
{
	//int i;

 	SetGraphicsMode(hdc, GM_ADVANCED);

	Level.Draw(hdc, paint_area);
	Border.Draw(hdc, paint_area);
	Platform.Draw(hdc, paint_area);
	Ball.Draw(hdc, paint_area);

	// test speed redraw platform_meltdown
	 
	/*COLORREF pixel;
	for (i = 0; i < 81 * 22 * 10; i++)
	{ 
		pixel = GetPixel(hdc, 100, 100);
		SetPixel(hdc, 100, 100, pixel);  
	}*/

	 // test speed redram - 2

	/*for(i=0; i<81 * 1000; i++)
	{
	MoveToEx(hdc, 100, 555, 0);
	SelectObject(hdc, AsConfig::BG_Pen);
	LineTo(hdc, 100, 557);

	SelectObject(hdc, AsConfig::Brick_Purple_Pen);
	LineTo(hdc, 100, 572);

	SelectObject(hdc, AsConfig::BG_Pen);
	LineTo(hdc, 100, 577);
	}
	int yy = 0*/

}
	
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	int offset_x_platform = 32;

	// Platform movement from pressing WINDOW_Keys_Virtual
	switch (key_type)
	{
	case EKT_Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos <= AsConfig::Border_X_Offset)
			Platform.X_Pos = AsConfig::Border_X_Offset;

		Platform.Redraw_Platform();
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;
		if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + offset_x_platform)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + offset_x_platform;

		Platform.Redraw_Platform();
		break;

	case EKT_Space:
		if(Platform.Get_State() == EPS_Ready)
		{
		Ball.Set_State(EBS_Normal, Platform.X_Pos - AsConfig::Offset_X_Ball_For_Ready + Platform.Width / 2);
		Platform.Set_State(EPS_Normal);
		}
		break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	++AsConfig::Tick_Current_Timer;

	switch (Game_State)
	{
	case EGS_Test_Mode:
		Ball.Set_Test();
		Game_State = EGS_Play_Level;
		break;

	case EGS_Play_Level:
		Ball.Move();

		if (Ball.Get_State() == EBS_Missing)
		{
			Game_State = EGS_Missing_Ball;
			Platform.Set_State(EPS_Meltdown);
		}

		if (Ball.Test_Finish() )
		Game_State = EGS_Test_Mode;
		break;

	case EGS_Missing_Ball:
		
		if (Platform.Get_State() == EPS_Missing)
		{
		Game_State = EGS_Replay_Level;
		Platform.Set_State(EPS_Roll_In);
		}
		break;

	case EGS_Replay_Level:
		if (Platform.Get_State() == EPS_Ready)
		{
			Game_State = EGS_Play_Level;
			Ball.Set_State(EBS_Ready, Platform.X_Pos + Platform.Width / 2);
		}
		break;
	}

	Platform.Act();
	Level.Act();

	//			if (AsConfig::Tick_Current_Timer % 10 == 0)
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
