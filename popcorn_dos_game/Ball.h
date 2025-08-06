#pragma once

#include "Config.h"
#include "Level.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_Normal,
	EBS_Missing,
	EBS_Ready
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();
	void Draw(HDC hdc, RECT& paint_area);
	void Move(ALevel *level, int platform_x_pos, int platform_width);
	EBall_State Get_State();
	void Set_State(EBall_State new_state, int x_pos);



	static const int Ball_Y = 398;
	static const int Ball_Size = 14;

private:
	void Redraw_Ball();

	EBall_State Ball_State;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush; 
	RECT Ball_Rect, Prev_Ball_Rect;
	
	double Ball_Speed;
	double Ball_Direction;
	double Ball_X_Pos, Ball_Y_Pos;
	int Ball_X_Offset, Ball_Y_Offset;
	static const double Start_Ball_Y_Pos;
	static const int Start_Ball_X_Pos = 388;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------