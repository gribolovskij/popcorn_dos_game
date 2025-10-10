#pragma once

#include "Config.h"
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
	EBT_Teleport,
	EBT_Reklama,
	EBT_FourHits,
	EBT_ThreeHits,
	EBT_TwoHits,
	EBT_Titan,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AAction_Brick
{
public:
	AAction_Brick(Ebrick_Type brick_Type, int level_x, int level_y);

	void Draw(HDC hdc, RECT& paint_area);
	void Act();
	bool Is_Finished();
	static void Setup_Colors();

private:
	Ebrick_Type brick_Type;

	RECT brick_rect;
	HPEN pen;
	HBRUSH brush;

	int Fade_Brick;

	static unsigned char Get_Fading_Channel_Color(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush);

	static const int Fade_Brick_Step = 80;

	static HPEN Fading_Blue_Brick_Pens[Fade_Brick_Step];
	static HBRUSH Fading_Blue_Brick_Brushes[Fade_Brick_Step];
	static HPEN Fading_Purple_Brick_Pens[Fade_Brick_Step];
	static HBRUSH Fading_Purple_Brick_Brushes[Fade_Brick_Step];
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
