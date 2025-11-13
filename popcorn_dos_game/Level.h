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
//
//
//
//-------------------class AFalling_Letter
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AFalling_Letter: public AGraphics_Object
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	const ELetter_Type Letter_Type;
	const EBrick_Type Brick_Type;

	bool Got_Hit;

private:
	RECT Letter_Cell, Prev_Letter_Cell;
	int X, Y;
	int Rotation_Step;
	int Next_Rotation_Tick;

	static const int Tick_Per_Step = 4;

	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc);
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//
//-------------------class ALevel
//
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

	RECT Level_Rect;

private:
	bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos);

	void On_Hit(int brick_x, int brick_y);
	void Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type);
	void Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);

	RECT paint_area;

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Y_High, Current_Brick_Y_Low;

	char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];

	int Action_Brick_Count;
	AAction_Brick *Action_Brick[AsConfig::Max_Action_Brick_Count];

	int Falling_Brick_Count;
	AFalling_Letter *Falling_Letter[AsConfig::Max_Falling_Letter_Count];


	AGraphics_Object **Graphics_Objects;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
