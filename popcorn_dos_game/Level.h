#pragma once

#include "Action_Brick.h"
#include "Ball.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
	ELT_None,
	ELT_O,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ALevel: public AHit_Checker
{
public:
	ALevel();

	void Init();
	void Act();
	void Draw(HDC hdc, RECT& paint_area);
	void Set_Current_Level(char Level[AsConfig::Level_Height][AsConfig::Level_Width]);

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
	static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

	//AAction_Brick Action_Brick;

	RECT Level_Rect;

private:
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos);

	void Add_Active_Brick(int brick_x, int brick_y);
	void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	HPEN Purple_Brick_Pen, Blue_Brick_Pen, Letter_Pen;
	HBRUSH Purple_Brick_Brush, Blue_Brick_Brush;

	RECT paint_area;

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Y_High, Current_Brick_Y_Low;

	char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];
	AAction_Brick *Action_Brick[AsConfig::Max_Action_Brick_Count];

	int Action_Brick_Count;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
