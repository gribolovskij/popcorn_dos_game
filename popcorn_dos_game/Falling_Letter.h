#pragma once

#include "Action_Brick.h"

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
class AFalling_Letter : public AGraphics_Object
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
