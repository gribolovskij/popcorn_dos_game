#include "Action_Brick.h"

// AAction_Brick
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HPEN AAction_Brick::Fading_Blue_Brick_Pens[Fade_Brick_Step];
HBRUSH AAction_Brick::Fading_Blue_Brick_Brushes[Fade_Brick_Step];
HPEN AAction_Brick::Fading_Purple_Brick_Pens[Fade_Brick_Step];
HBRUSH AAction_Brick::Fading_Purple_Brick_Brushes[Fade_Brick_Step];
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AAction_Brick::AAction_Brick(Ebrick_Type brick_Type)
	: Fade_Brick(0), brick_rect{}, brick_Type(brick_Type), pen(0), brush(0)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AAction_Brick::Draw(HDC hdc)
{
	HPEN pen = 0;
	HBRUSH brush = 0;

	switch (brick_Type)
	{
	case EBT_Blue:
	pen = Fading_Blue_Brick_Pens[Fade_Brick];
	brush = Fading_Blue_Brick_Brushes[Fade_Brick];
	break;

	case EBT_Purple:
	pen = Fading_Purple_Brick_Pens[Fade_Brick];
	brush = Fading_Purple_Brick_Brushes[Fade_Brick];
	break;
	}

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
	if (Fade_Brick < Fade_Brick_Step - 1)
	{
		++Fade_Brick;
		InvalidateRect(Hwnd, &brick_rect, FALSE);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AAction_Brick::Setup_Colors()
{
	int i;
	for (i = 0; i < Fade_Brick_Step; i++)
	{
		Get_Fading_Color(AsConfig::Gray_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);

		Get_Fading_Color(AsConfig::Orange_Brick_Color, i, Fading_Purple_Brick_Pens[i], Fading_Purple_Brick_Brushes[i]);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned char AAction_Brick::Get_Fading_Channel_Color(unsigned char color, unsigned char bg_color, int step)
{
	return color - step * (color - bg_color) / Fade_Brick_Step - 1;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AAction_Brick::Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel_Color(color.R, AsConfig::BG_Color.R, step);
	g = Get_Fading_Channel_Color(color.B, AsConfig::BG_Color.G, step);
	b = Get_Fading_Channel_Color(color.R, AsConfig::BG_Color.B, step);

	AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
