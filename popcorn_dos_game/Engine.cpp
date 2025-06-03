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
HPEN Letter_Pen, Purple_Brick_Pen, Blue_Brick_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen, BG_Pen;
HBRUSH Purple_Brick_Brush, Blue_Brick_Brush, Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush, BG_Brush;

const int Global_Scale = 1;
const int Brick_Width = 61;
const int Brick_Height = 23;
const int Cell_Width = 64;
const int Cell_Height = 26;
const int Level_X_Offset = 23;
const int Level_Y_Offset = 13;
const int Circle_Size = 20;
const int Volume_Rectangle = 59;
const int Y_Letter = 8;
const int Platform_Y_Pos = 550;
const int Platform_Height = 25;

int Platform_X_Pos = 0;
int Platform_X_Step = 10;
int Platform_Width = 115;
int Inner_Width = 40;

RECT Platform_Rect, Prev_Platform_Rect;

char Level_01[14][12] =
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
{// Функция реализации цвета RGB одной строкой
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Redraw_Platform()
{	
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = Platform_X_Pos * Global_Scale;
	Platform_Rect.top = Platform_Y_Pos * Global_Scale;
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
	Redraw_Platform();
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

	 //	Выводим фон
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = (1.0 - fabs(xForm.eM22)) * 12;
		back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Volume_Rectangle, brick_half_height - back_part_offset);

	// Выводим передний план
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, brick_half_height_foreground, Volume_Rectangle, -brick_half_height_foreground);

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
//	Вывод всех кирпичей уровня
{
	int i,j;

	for (i = 0; i < 14; i++)	
		for (j = 0; j< 12; j++) 
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (Ebrick_Type)Level_01[i][j]);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
//	Рисуем платформу
{
	// Чистим экран при перемещение платформы
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.right, Prev_Platform_Rect.top, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + Circle_Size* Global_Scale, y + Circle_Size * Global_Scale);
	Ellipse(hdc, x + (Circle_Size+Inner_Width), y, x + ((Circle_Size*2)+Inner_Width), y+ Circle_Size);

	// 2. Рисуем среднюю часть
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y+Global_Scale*2, 10 * Global_Scale, 32 * Global_Scale);
						//x + Inner_Width Расстояние между шариками, может увеличиваться!!!

	// 3. Рисуем среднюю часть
	SelectObject(hdc, Arc_Pen);
	Arc	(hdc, x+13, y+13, x+3, y+2, x+10, 489, 179, 489 );
	// highlight need correct!
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
//	Отрисовка экрана игры
{
	Draw_Level(hdc);

	Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);

	//int i;
	//for (i = 0; i < 16; i++)
	//{
	//	Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 200, EBT_Blue, ELT_O ,i);
	//	Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 130, EBT_Purple, ELT_O,  i);

	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type)
{

	switch (key_type)
	{
	case EKT_Left:
		Platform_X_Pos -= Platform_X_Step;
		Redraw_Platform();
		break;

	case EKT_Right:
		Platform_X_Pos += Platform_X_Step;
		Redraw_Platform();
		break;

	case EKT_Space:

		break;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
