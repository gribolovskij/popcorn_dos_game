#pragma once

#include "Falling_Letter.h"
#include "Ball.h"

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

	void Draw(HDC hdc, RECT& paint_area);
	void Init();
	void Act();
	void Set_Current_Level(char Level[AsConfig::Level_Height][AsConfig::Level_Width]);

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
	static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

	RECT Level_Rect;

private:
	void ObjectsToAct(int object_max_count, AGraphics_Object **object_array);
	void Draw_Graphics_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array ,int object_max_count);
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
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
