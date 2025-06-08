#pragma once

#include <Windows.h>

const int Timer_ID = WM_USER + 1;

enum EKey_Type
{
	EKT_Left,
	EKT_Right,
	EKT_Space,
};

// Функции интерфейса модуля
void Init_Engine(HWND hwnd);
void Draw_Frame(HDC hdc, RECT& paint_area);
int On_Key_Down(EKey_Type key_type);
int On_Timer();
