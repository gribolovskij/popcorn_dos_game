#pragma once

#include "Config.h"
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EBrick_Type
{
	EBT_None,

	EBT_Yellow,
	EBT_Blue,
	EBT_Teleport,
	EBT_Reklama,
	EBT_FourHits,
	EBT_ThreeHits,
	EBT_TwoHits,
	EBT_Titan,
	EBT_Pasachut
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//
//-------------------class AGraphics_Object
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
	virtual void Draw(HDC hdc, RECT& paint_area) = 0;
	virtual void Act() = 0;
	virtual bool Is_Finished() = 0;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//
//-------------------class AAction_Brick
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AAction_Brick: public AGraphics_Object
{
public:
	AAction_Brick(EBrick_Type brick_Type, int level_x, int level_y);

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Setup_Colors();

private:
	EBrick_Type brick_Type;

	int Fade_Brick;

	RECT brick_rect;
	HPEN pen;
	HBRUSH brush;

	static unsigned char Get_Fading_Channel_Color(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush);

	static const int Fade_Brick_Step = 80;

	static HPEN Fading_Blue_Brick_Pens[Fade_Brick_Step];
	static HBRUSH Fading_Blue_Brick_Brushes[Fade_Brick_Step];
	static HPEN Fading_Purple_Brick_Pens[Fade_Brick_Step];
	static HBRUSH Fading_Purple_Brick_Brushes[Fade_Brick_Step];
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
