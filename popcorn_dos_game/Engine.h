#pragma once

#include <Windows.h>

#include "Border.h"
#include "Platform.h"
#include "Level.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
	EKT_Left,
	EKT_Right,
	EKT_Space,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EGame_State
{
	EGS_Test_Mode,
	EGS_Play_Level,
	EGS_Missing_Ball,
	EGS_Replay_Level
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

private:
	void Act();
	void Take_Falling_Letter(AFalling_Letter *falling_letter);
	EGame_State Game_State;

	ABall Ball;
	AsLevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
