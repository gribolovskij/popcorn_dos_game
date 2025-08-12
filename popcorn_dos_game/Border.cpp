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
	int i;

	// 1. Line border left
	for (i = 0; i < 60; i++)
		Draw_Element(hdc, 5, 1 + i * 12, false);

	// 2. Line border right					
	for (i = 0; i < 60; i++)				
		Draw_Element(hdc, 788, 1 + i * 12, false);

	// 3. Line top
	for (i = 0; i < 79; i++)
		Draw_Element(hdc, 8 + i * 10, 0, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
//	Draw frame element
{
	//	 General blue color line border
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if(top_border)
		Rectangle(hdc, x, y+1, x + 13, y + 17);
	else
		Rectangle(hdc, x + 1, y, x + 17, y  + 13);

	//	 White color border
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if(top_border)
		Rectangle(hdc, x, y, x + 14, y + 4);
	else
		Rectangle(hdc, x , y , x + 5, y + 14);

	//	 Perforation
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	if(top_border)
		Rectangle(hdc, x + 4, y + 7, x + 8, y + 11);
	else
		Rectangle(hdc, x + 7, y + 4, x + 11, y + 8);
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
