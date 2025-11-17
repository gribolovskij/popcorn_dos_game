#include "Falling_Letter.h"


//-------------------AFalling_Letter
// 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
	: Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Got_Hit(false), Rotation_Step(2), Next_Rotation_Tick(AsConfig::Tick_Current_Timer + Tick_Per_Step)
{
	Letter_Cell.left = X;
	Letter_Cell.top = Y;
	Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width;
	Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height;

	Prev_Letter_Cell = Letter_Cell;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
	Prev_Letter_Cell = Letter_Cell;

	Y += 5;
	Letter_Cell.top += 5;
	Letter_Cell.bottom += 5;


	if (AsConfig::Tick_Current_Timer >= Next_Rotation_Tick)
	{
		++Rotation_Step;
		Next_Rotation_Tick += Tick_Per_Step;
	}

	InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
	{
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);

		Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
		Draw_Brick_Letter(hdc);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
	if (Got_Hit || Letter_Cell.top >= AsConfig::Max_Y_Pos)

		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
//	Drawing Falling Break with Letter
{
	bool switch_color;
	double offset;
	// Converting step to rotation angle
	double rotation_angle;
	int brick_half_height = (AsConfig::Brick_Height / 2);
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xForm, old_xForm;

	if (!(Brick_Type == EBT_Blue || Brick_Type == EBT_Yellow))
		// Falling letters can only be from this type of brick
		return;

	// Adjust the rotation step and rotation angle
	// Take the remainder of the division by 16 and put it back into the variable
	Rotation_Step = Rotation_Step % 16;

	if (Rotation_Step < 8)
		// Delayed initialization
		rotation_angle = 2.0 * M_PI / 16.0 * (double)Rotation_Step;
	else
		rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - Rotation_Step);

	if (Rotation_Step > 4 && Rotation_Step <= 12)
	{
		if (Brick_Type == EBT_Blue)
			switch_color = true;
		else
			switch_color = false;
		//	switch_color = brick_type == EBT_Blue;
	}
	else
	{
		if (Brick_Type == EBT_Yellow)
			switch_color = true;
		else
			switch_color = false;
	}
	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (Rotation_Step == 4 || Rotation_Step == 12)
	{
		// Display the background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, X, Y + brick_half_height - 3, X + AsConfig::Volume_Rectangle, Y + brick_half_height);

		// Display the foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, X, Y + brick_half_height, X + AsConfig::Volume_Rectangle, Y + brick_half_height + 3);
	}
	else
	{
		// Set up the letter "flip" matrix
		xForm.eM11 = 1.0f;
		xForm.eM12 = 0.0f;
		xForm.eM21 = 0.0f;
		xForm.eM22 = (float)cos(rotation_angle);
		xForm.eDx = (float)X;
		xForm.eDy = (float)Y + (float)brick_half_height;

		GetWorldTransform(hdc, &old_xForm);
		SetWorldTransform(hdc, &xForm);

		//	Deduced background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = (1.0 - fabs(xForm.eM22)) * 12;
		back_part_offset = (int)round(offset);
		RoundRect(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Volume_Rectangle, brick_half_height - back_part_offset, 10, 32);

		// Deduced foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		RoundRect(hdc, 0, brick_half_height, AsConfig::Volume_Rectangle, -brick_half_height, 10, 32);

		if (Rotation_Step > 4 && Rotation_Step <= 12)
		{
			if (Letter_Type == ELT_O)
			{
				SelectObject(hdc, AsConfig::Letter_Pen);
				Ellipse(hdc, AsConfig::Circle_Size, -AsConfig::Y_Letter, AsConfig::Circle_Size * 2, AsConfig::Y_Letter);
			}
		}
		SetWorldTransform(hdc, &old_xForm);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
	if (is_switch_color)
	{
		front_pen = AsConfig::Blue_Brick_Pen;
		front_brush = AsConfig::Blue_Brick_Brush;
		back_pen = AsConfig::Purple_Brick_Pen;
		back_brush = AsConfig::Purple_Brick_Brush;
	}
	else
	{
		front_pen = AsConfig::Purple_Brick_Pen;
		front_brush = AsConfig::Purple_Brick_Brush;
		back_pen = AsConfig::Blue_Brick_Pen;
		back_brush = AsConfig::Blue_Brick_Brush;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

