#pragma once


#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
	ELT_None,
	ELT_O,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
	ALevel();

	void Init();
	void Check_Level_Hit_Brick(int &next_y_pos, double &ball_direction);
	void Draw(HDC hdc);

	RECT Level_Rect;

private:

	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	HPEN Purple_Brick_Pen, Blue_Brick_Pen, Letter_Pen;
	HBRUSH Purple_Brick_Brush, Blue_Brick_Brush;
	RECT paint_area;

	static const int Brick_Width = 61;
	static const int Brick_Height = 23;

};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
