#pragma once

#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_Normal,
	EBS_Missing,
	EBS_Ready
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Draw(HDC hdc, RECT& paint_area);
	void Move();
	EBall_State Get_State();
	void Set_State(EBall_State new_state, int x_pos);

	double Ball_Direction;

	static void Add_Hit_Checkers(AHit_Checker *hit_checker);

	static const int Ball_Y = 398;
	static const int Ball_Size = 14;
	static const double Radius;

private:
	void Redraw_Ball();

	EBall_State Ball_State;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush; 
	RECT Ball_Rect, Prev_Ball_Rect;
	
	double Ball_Speed;
	double Rest_Distance;
	double Center_X_Pos, Center_Y_Pos;
	int Ball_X_Offset, Ball_Y_Offset;
	static const double Start_Ball_Y_Pos;

	static const int Start_Ball_X_Pos = 388;
	static int Count_Hit_Checkers;
	static AHit_Checker *Hit_Checkers[3];
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------