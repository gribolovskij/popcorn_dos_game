#include "Border.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Init()
{
	AsEngine::Create_Pen_Brush(108, 251, 247, Border_Blue_Pen, Border_Blue_Brush);
	AsEngine::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN &bg_pen, HBRUSH &bg_brush)
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
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

	if(top_border)
		Rectangle(hdc, x + 4, y + 7, x + 8, y + 11);
	else
		Rectangle(hdc, x + 7, y + 4, x + 11, y + 8);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT& paint_area, HPEN &bg_pen, HBRUSH &bg_brush)
//	Drawing screen game
{
	int i;

	// 1. Line border left
	for (i = 0; i < 60; i++)
		Draw_Element(hdc, 5, 1 + i * 12, false, bg_pen, bg_brush);

	// 2. Line border right
	for (i = 0; i < 60; i++)
		Draw_Element(hdc, 788, 1 + i * 12, false, bg_pen, bg_brush);

	// 3. Line top
	for (i = 0; i < 79; i++)
		Draw_Element(hdc, 8 + i * 10, 0, true, bg_pen, bg_brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

