#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned  char r, unsigned  char g, unsigned  char b)
	:R(r), G(g), B(b)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
	return RGB(R, G, B);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsConfig::Level_Has_Floor = false;
int AsConfig::Tick_Current_Timer = 0;

const AColor AsConfig::Gray_Brick_Color(112, 146, 190); 
const AColor AsConfig::Orange_Brick_Color(255, 182, 89); 
const AColor AsConfig::BG_Color(3, 105, 24);
const double AsConfig::Step_Move = 1.0;

	HPEN AsConfig::BG_Pen;
	HBRUSH AsConfig::BG_Brush;
	HWND AsConfig::Hwnd;

	HPEN AsConfig::Purple_Brick_Pen, AsConfig::Blue_Brick_Pen, AsConfig::Letter_Pen;
	HBRUSH AsConfig::Purple_Brick_Brush, AsConfig::Blue_Brick_Brush;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
	AsConfig::Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	AsConfig::Create_Pen_Brush(AsConfig::Gray_Brick_Color, AsConfig::Blue_Brick_Pen, AsConfig::Blue_Brick_Brush);
	AsConfig::Create_Pen_Brush(AsConfig::Orange_Brick_Color, AsConfig::Purple_Brick_Pen, AsConfig::Purple_Brick_Brush);
	AsConfig::Create_Pen_Brush(BG_Color, BG_Pen, BG_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN &pen, HBRUSH &brush)
{
	//Function to implement RGB color in one line
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush)
{
	//Function to implement RGB color in one line
	pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
	brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)
{
	// Calculates a pseudo-random number in the range [0, .. range - 1]
	return rand() * range / RAND_MAX;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
