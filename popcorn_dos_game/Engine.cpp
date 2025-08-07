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
	AsConfig::Hwnd = hwnd;

	AAction_Brick::Setup_Colors();

	Level.Init();
	Platform.Init();
	Ball.Init();
	Border.Init();

	Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);

	Platform.Set_State(EPS_Normal);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000/AsConfig::FPS, 0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
//	Drawing screen game
{
	Border.Draw(hdc, paint_area);
	Ball.Draw(hdc, paint_area);
	Level.Draw(hdc);

	Platform.Draw(hdc, paint_area);

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

		if (Platform.X_Pos <= AsConfig::Border_X_Offset)
			Platform.X_Pos = AsConfig::Border_X_Offset;

		Platform.Redraw_Platform();
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;
		if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 32)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 32;

		Platform.Redraw_Platform();
		break;

	/*case EKT_Space:	// My Logic
		if (Ball.Ball_State == EBS_Missing || Ball.Ball_State == EBS_Ready)
		Game_State = EGS_Play_Level;
		Ball.Ball_Speed = 8.0;
		break; */

	case EKT_Space:
		if(Platform.Get_State() == EPS_Ready)
		{
		Ball.Set_State(EBS_Normal, ABall::Ball_Y);
		Platform.Set_State(EPS_Normal);
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	++AsConfig::Tick_Current_Timer;

	switch (Game_State)
	{
	case EGS_Play_Level:
		Ball.Move(Platform.X_Pos, Platform.Width, &Level, &Border);

		if (Ball.Get_State() == EBS_Missing)
		{
			Game_State = EGS_Missing_Ball;
			Platform.Set_State(EPS_Meltdown);
		}
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

	//	Level.Action_Brick.Act();
	//			if (AsConfig::Tick_Current_Timer % 10 == 0)
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
