#include "Action_Brick.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AAction_Brick::AAction_Brick()
	: Fade_Step(0), pen(0), brush(0), brick_rect{}
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AAction_Brick::Draw(HDC hdc)
{
	AsConfig::Create_Pen_Brush(112 - (112 / Fade_Brick) * Fade_Step, 146 - (146 / Fade_Brick) * Fade_Step, 190 - (198 / Fade_Brick) * Fade_Step, pen, brush);

	brick_rect.left = AsConfig::Brick_Height;
	brick_rect.top = AsConfig::Fault_Variable;
	brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height;
	brick_rect.right = brick_rect.left + AsConfig::Brick_Width;

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right, brick_rect.bottom, 10 * AsConfig::Global_Scale, 32 * AsConfig::Global_Scale);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AAction_Brick::Act(HWND Hwnd)
{
	if (Fade_Step < Fade_Brick)
	{
		++Fade_Step;
		InvalidateRect(Hwnd, &brick_rect, FALSE);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------