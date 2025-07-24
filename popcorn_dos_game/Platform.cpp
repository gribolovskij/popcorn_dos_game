#include "Platform.h"


// AsPlatform
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: X_Pos(AsConfig::X_Offset), Width(115), Inner_Width(40), Arc_Pen(0), Arc_Brush(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0), Platform_Rect{}, Prev_Platform_Rect{}
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	AsConfig::Create_Pen_Brush(155, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(249, 100, 0, Platform_Inner_Pen, Platform_Inner_Brush);
	AsConfig::Create_Pen_Brush(255, 255, 255, Arc_Pen, Arc_Brush);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND Hwnd)
{	
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = X_Pos;
	Platform_Rect.top = AsConfig::Platform_Y_Pos;
	Platform_Rect.right = Platform_Rect.left + Width;
	Platform_Rect.bottom = Platform_Rect.top + Height;

	InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
//	Drawing platform
{
	int x = X_Pos;
	int y = AsConfig::Platform_Y_Pos;

	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	// Cleen the window, when moving the platform
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Drawing lateral circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + AsConfig::Circle_Size * AsConfig::Global_Scale, y + AsConfig::Circle_Size * AsConfig::Global_Scale);
	Ellipse(hdc, x + (AsConfig::Circle_Size + Inner_Width), y, x + ((AsConfig::Circle_Size*2)+Inner_Width), y + AsConfig::Circle_Size);

	// 2. Drawing inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y + AsConfig::Global_Scale*2, 10 * AsConfig::Global_Scale, 32 * AsConfig::Global_Scale);

	// 3. Drawing highlight
	SelectObject(hdc, Arc_Pen);
	Arc	(hdc, x+13, y+13, x+3, y+2, x+10, y - 61, x - 121, y - 61 );
	//	good arc!! very good nice!!!
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





