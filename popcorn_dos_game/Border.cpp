#include "Border.h"

AsBorder::AsBorder()
	: Border_Blue_Pen(0), Border_White_Pen(0), Border_Blue_Brush(0), Border_White_Brush(0)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Init()
{
	AsConfig::Create_Pen_Brush(108, 251, 247, Border_Blue_Pen, Border_Blue_Brush);
	AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
//	Drawing screen game
{
	int magic_number1 = 1, magic_number2 = 5, magic_number3 = 12, magic_number4 = 788, magic_number5 = 8, magic_number6 = 10, magic_number7 = 0;
	int i;

	// 1. Line border left
	for (i = 0; i < 60; i++)
		Draw_Element(hdc, magic_number2, magic_number1 + i * magic_number3, false);

	// 2. Line border right	
	for (i = 0; i < 60; i++)
		Draw_Element(hdc, magic_number4, magic_number1 + i * magic_number3, false);

	// 3. Line top
	for (i = 0; i < 79; i++)
		Draw_Element(hdc, magic_number5 + i * magic_number6, magic_number7, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
//	Draw frame element
{
	int tb_correct_draw1 = 17, tb_correct_draw2 = 13, tb_correct_draw3 = 1, tb_correct_draw4 = 14, tb_correct_draw5 = 5, tb_correct_draw6 = 4, tb_correct_draw7 = 7, tb_correct_draw8 = 8, 	tb_correct_draw9 = 11;

	//	 General blue color line border
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if(top_border)
		Rectangle(hdc, x, y+tb_correct_draw3, x + tb_correct_draw2, y + tb_correct_draw1);
	else
		Rectangle(hdc, x + tb_correct_draw3, y, x + tb_correct_draw1, y  + tb_correct_draw2);

	//	 White color border
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if(top_border)
		Rectangle(hdc, x, y, x + tb_correct_draw4, y + tb_correct_draw5);
	else
		Rectangle(hdc, x , y , x + tb_correct_draw5, y + tb_correct_draw4);

	//	 Perforation
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	if(top_border)
		Rectangle(hdc, x + tb_correct_draw6, y + tb_correct_draw7, x + tb_correct_draw8, y + tb_correct_draw9);
	else
		Rectangle(hdc, x + tb_correct_draw7, y + tb_correct_draw6, x + tb_correct_draw9, y + tb_correct_draw8);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	bool got_hit = false;
	//	1. Correction position when reflecting from the frame
	if (next_x_pos - ball -> Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball -> Reflect(false);
	}
	if (next_y_pos - ball -> Radius  < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball -> Reflect(true);
	}
	if (next_x_pos + ball -> Radius > AsConfig::Max_X_Pos)
	{
		got_hit = true;
		ball -> Reflect(false);
	}
	if (next_y_pos + ball -> Radius > AsConfig::Max_Y_Pos)
	{
		if (AsConfig::Level_Has_Floor)
		{
			got_hit = true;
			ball -> Reflect(true);
		}
		else

			if (next_y_pos + ball -> Radius > AsConfig::Max_Y_Pos + ball -> Radius * 4.0)
				ball -> Set_State(EBS_Missing, next_x_pos);
	}			
	return got_hit;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
