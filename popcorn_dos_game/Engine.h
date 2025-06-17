#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
	EKT_Left,
	EKT_Right,
	EKT_Space,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
	ELT_None,

	ELT_O,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();
	
	// Функции интерфейса модуля
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	static const int Level_Width = 12;		// Width brick
	static const int Level_Height = 14;	// Height brick

private:
	void Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN& pen, HBRUSH& brush);
	void Redraw_Platform();
	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Draw_Level(HDC hdc);
	void Draw_Platform(HDC hdc, int x, int y);
	void Draw_Ball(HDC hdc, RECT& paint_area);
	void Draw_Border(HDC hdc, int x, int y, bool top_border);
	void Draw_Bounce(HDC hdc, RECT& paint_area);
	void Check_Hit_Brick(int& next_y_pos);
	void Move_Ball();

	HWND Hwnd;
	HPEN Letter_Pen, Purple_Brick_Pen, Blue_Brick_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen, BG_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
	HBRUSH Purple_Brick_Brush, Blue_Brick_Brush, Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush, BG_Brush, Ball_Brush, Border_Blue_Brush, Border_White_Brush;

	int Platform_X_Pos;
	int Platform_Width;
	int Platform_Inner_Width;
	int Ball_X_Pos, Ball_Y_Pos;
	int Ball_X_Offset, Ball_Y_Offset;
	double Ball_Speed, Ball_Direction;				//M_PI_4 = 45 градусов

	RECT Platform_Rect, Prev_Platform_Rect;
	RECT Level_Rect;
	RECT Ball_Rect, Prev_Ball_Rect;

	static const int Global_Scale = 1;
	static const int Brick_Width = 61;
	static const int Brick_Height = 23;
	static const int Cell_Width = 64;
	static const int Cell_Height = 26;
	static const int Level_X_Offset = 23;		// Position start level
	static const int Level_Y_Offset = 13;		// Position start level
	static const int Circle_Size = 20;
	static const int Volume_Rectangle = 59;
	static const int Y_Letter = 8;
	static const int Platform_Y_Pos = 550;
	static const int Platform_Height = 25;
	static const int Platform_X_Step = 20;
	static const int Ball_Size = 14;
	static const int Max_X_Pos = (Level_X_Offset + Cell_Width * Level_Width);
	static const int Max_Y_Pos = 632;
	static const int Border_X_Offset = 22;
	static const int Border_Y_Offset = 17;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
