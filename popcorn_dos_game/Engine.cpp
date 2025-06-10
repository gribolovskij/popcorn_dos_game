#include "Engine.h"
#define _USE_MATH_DEFINES
#include "math.h"


enum ELetter_Type
{
	ELT_None,

	ELT_O,
};

enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};

HWND Hwnd;
HPEN Letter_Pen, Purple_Brick_Pen, Blue_Brick_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen, BG_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
HBRUSH Purple_Brick_Brush, Blue_Brick_Brush, Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush, BG_Brush, Ball_Brush, Border_Blue_Brush, Border_White_Brush;

const int Global_Scale = 1;
const int Brick_Width = 61;
const int Brick_Height = 23;
const int Cell_Width = 64;
const int Cell_Height = 26;
const int Level_X_Offset = 23;		// Position start level
const int Level_Y_Offset = 13;		// Position start level
const int Circle_Size = 20;
const int Volume_Rectangle = 59;
const int Y_Letter = 8;
const int Platform_Y_Pos = 550;
const int Platform_Height = 25;
const int Platform_X_Step = 20;
const int Level_Width = 12;		// Width brick
const int Level_Height = 14;	// Height brick
const int Ball_Size = 18;
const int Max_X_Pos = (Level_X_Offset + Cell_Width * Level_Width);
const int Max_Y_Pos = 632 - Ball_Size;

const int Border_X_Offset = 22;
const int Border_Y_Offset = 67;

int Platform_X_Pos = Border_X_Offset;
int Platform_Width = 115;
int Platform_Inner_Width = 40;

int Ball_X_Pos = 64, Ball_Y_Pos = 500;

//	Speed ball
int Ball_X_Offset = 3, Ball_Y_Offset = -3;
double Ball_Speed = 4.0, Ball_Direction = M_PI - M_PI_4;				//M_PI_4 = 45 градусов

RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;
RECT Ball_Rect, Prev_Ball_Rect;

char Level_01[Level_Height][Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN &pen, HBRUSH &brush)
{// Функция реализации цвета RGB одной строкой //	Function to implement RGB color in one line
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Redraw_Platform()
{	
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = Platform_X_Pos;
	Platform_Rect.top = Platform_Y_Pos;
	Platform_Rect.right = Platform_Rect.left + Platform_Width;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height;

	InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Init_Engine(HWND hwnd)
//	Функция инициализации |=|=|=| Настройка игры при старте
{
	Hwnd = hwnd;

	Create_Pen_Brush(3, 105, 24, BG_Pen, BG_Brush);

	Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	Create_Pen_Brush(112, 146, 190, Blue_Brick_Pen, Blue_Brick_Brush);
	Create_Pen_Brush(255, 182, 89, Purple_Brick_Pen, Purple_Brick_Brush);
	Create_Pen_Brush(155, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	Create_Pen_Brush(249, 100, 0, Platform_Inner_Pen, Platform_Inner_Brush);
	Create_Pen_Brush(255, 255, 255, Arc_Pen, Arc_Brush);
	Create_Pen_Brush(255, 0, 0, Ball_Pen, Ball_Brush);
	Create_Pen_Brush(108, 251, 247, Border_Blue_Pen, Border_Blue_Brush);
	Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);

	Level_Rect.left = Level_X_Offset;
	Level_Rect.top = Level_Y_Offset ;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height;

	Redraw_Platform();

	SetTimer(Hwnd, Timer_ID, 1, 0);

	}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type)
//	Вывод кирпича
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_None:	return;

	case EBT_Purple:
		pen = Purple_Brick_Pen;								 // Создаем цвет для 2-ого кирпича
		brush = Purple_Brick_Brush;

		break;

	case EBT_Blue:
		pen = Blue_Brick_Pen;								 // Создаем цвет для 1-ого кирпича
		brush = Blue_Brick_Brush;

		break;

	default: return;
	}
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * Global_Scale, y * Global_Scale, Brick_Width + x * Global_Scale, Brick_Height + y * Global_Scale, 10*Global_Scale, 32*Global_Scale);	

	// Кирпич - рисуем
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
//	
{
	if (is_switch_color)
	{
		front_pen = Blue_Brick_Pen;
		front_brush = Blue_Brick_Brush;

		back_pen = Purple_Brick_Pen;
		back_brush = Purple_Brick_Brush;
	}
	else
	{
		front_pen = Purple_Brick_Pen;
		front_brush = Purple_Brick_Brush;

		back_pen = Blue_Brick_Pen;
		back_brush = Blue_Brick_Brush;

	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type,int rotation_step)
//	Отрисовка падающего кирпича с буквой
{
	bool switch_color;
	double offset;
	double rotation_angle;		// Преобразование шага в угол поворота
	int brick_half_height = (Brick_Height / 2);
	int brick_half_height_foreground = (Circle_Size / 2);
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xForm, old_xForm;

	if (!(brick_type == EBT_Blue || brick_type == EBT_Purple))
		return;		// Падающие буквы могут быть только от кирпичей такого типа

	// Корректируем шаг вращения и угол поворота
	rotation_step = rotation_step % 16;											// Возьмём остаток от деления на 16 и поместим обратно в переменную

	if (rotation_step < 8)
		rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;				// Отложенная инициализация
	else
		rotation_angle = 2.0 * M_PI / 16 * (double)(8L - (long long)rotation_step);

	if (rotation_step > 4 && rotation_step <= 12)
	{
		if (brick_type == EBT_Blue)
			switch_color = true;
		else 
			switch_color = false;
									//	switch_color = brick_type == EBT_Blue;
	}
	else
	{
		if (brick_type == EBT_Purple)
			switch_color = true;
		else
			switch_color = false;
	}
Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (rotation_step == 4 || rotation_step == 12)
	{
		// Выводим фон
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - 3, x + Volume_Rectangle, y + brick_half_height);

		// Выводим передний план
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height , x + Volume_Rectangle, y + brick_half_height + 3);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);

		// Настраиваем матрицу "переворота" буквы
		xForm.eM11 = 1.0f;
		xForm.eM12 = 0.0f;
		xForm.eM21 = 0.0f;
		xForm.eM22 = (float)cos(rotation_angle);
		xForm.eDx  = (float)x;
		xForm.eDy  = (float)y + (float)brick_half_height;
		GetWorldTransform(hdc, &old_xForm);
		SetWorldTransform(hdc, &xForm);

	 //	Deduced background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = (1.0 - fabs(xForm.eM22)) * 12;
		back_part_offset = (int)round(offset);
		RoundRect(hdc, 0, -brick_half_height - back_part_offset, Volume_Rectangle, brick_half_height - back_part_offset, 10, 32);

	// Deduced foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		RoundRect(hdc, 0, brick_half_height_foreground, Volume_Rectangle, -brick_half_height_foreground, 10, 32);

		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELT_O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, Circle_Size, -Y_Letter, Circle_Size * 2, Y_Letter);
			}
		}

		SetWorldTransform(hdc, &old_xForm);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
//	Drawing all bricks level
{
	int i,j;

	for (i = 0; i < Level_Height; i++)
		for (j = 0; j< Level_Width; j++)
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (Ebrick_Type)Level_01[i][j]);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
//	Drawing platform
{
	// Cleen the window, when moving the platform
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Drawing lateral circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + Circle_Size* Global_Scale, y + Circle_Size * Global_Scale);
	Ellipse(hdc, x + (Circle_Size+Platform_Inner_Width), y, x + ((Circle_Size*2)+Platform_Inner_Width), y+ Circle_Size);

	// 2. Drawing inner part
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Platform_Inner_Width + 31), y+Global_Scale*2, 10 * Global_Scale, 32 * Global_Scale);

	// 3. Drawing highlight
	SelectObject(hdc, Arc_Pen);
	Arc	(hdc, x+13, y+13, x+3, y+2, x+10, y - 61, x - 121, y - 61 );
	//	good arc!! very good nice!!!
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Ball(HDC hdc, RECT &paint_area)
{
	//	1. Clean background
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

	//	2. Draw ball
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Border(HDC hdc, int x, int y, bool top_border)
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
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);

	if(top_border)
		Rectangle(hdc, x + 4, y + 7, x + 8, y + 11);
	else
		Rectangle(hdc, x + 7, y + 4, x + 11, y + 8);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Bounce(HDC hdc, RECT& paint_area)
//	Drawing screen game
{
		int i;

		// 1. Line border left
	for (i = 0; i < 60; i++)
		Draw_Border(hdc, 5, 1 + i * 12, false);

		// 2. Line border right
	for (i = 0; i < 60; i++)
		Draw_Border(hdc, 788, 1 + i * 12, false);

		// 3. Line top
	for (i = 0; i < 79; i++)
		Draw_Border(hdc, 8 + i * 10, 0, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc, RECT &paint_area)
//	Drawing screen game
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect));
	Draw_Level(hdc);

	if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect));
	Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);

	/*int i;
	for (i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 200, EBT_Blue, ELT_O, i);
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 130, EBT_Purple, ELT_O, i);
	}*/
	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) );
	Draw_Ball(hdc, paint_area);

	Draw_Bounce(hdc, paint_area);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type)
{
	// Platform movement from pressing WINDOW_Keys_Virtual
	switch (key_type)
	{
	case EKT_Left:
		Platform_X_Pos -= Platform_X_Step;

		if (Platform_X_Pos <= Border_X_Offset)
			Platform_X_Pos = Border_X_Offset;

		Redraw_Platform();
		break;

	case EKT_Right:
		Platform_X_Pos += Platform_X_Step;
		if (Platform_X_Pos >= Max_X_Pos - Platform_Width + 32)
			Platform_X_Pos = Max_X_Pos - Platform_Width + 32;

		Redraw_Platform();
		break;

	case EKT_Space:
		break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Move_Ball()
{
	int next_x_pos, next_y_pos;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction) );
	next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction) );

	//	1. Correction position when reflecting
	if (next_x_pos < Border_X_Offset)
	{
		next_x_pos = Level_X_Offset - (next_x_pos - Level_X_Offset);
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < Border_Y_Offset)
	{
		next_y_pos = Level_Y_Offset - (next_y_pos - Level_Y_Offset);
		Ball_Direction = -Ball_Direction;
	}
	if (next_x_pos > Max_X_Pos - Ball_Size)
	{
		next_x_pos = Max_X_Pos - (next_x_pos - Max_X_Pos);
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos > Max_Y_Pos)
	{
		next_x_pos = Max_X_Pos - (next_x_pos - Max_X_Pos);
		Ball_Direction = M_PI_2 - Ball_Direction;
	}
	//	2. Move the ball
	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;
	Ball_Rect.left = Ball_X_Pos;
	Ball_Rect.top = Ball_Y_Pos;
	Ball_Rect.right = Ball_Rect.left + Ball_Size;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size;

	InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int On_Timer()
{
	Move_Ball();

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
