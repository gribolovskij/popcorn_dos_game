#include "Engine.h"

enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};

HPEN Purple_Brick_Pen, Blue_Brick_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen;
HBRUSH Purple_Brick_Brush, Blue_Brick_Brush, Platform_Circle_Brush, Platform_Inner_Brush;

const int Global_Scale = 1;
const int Brick_Width = 61;
const int Brick_Height = 23;
const int Cell_Width = 64;
const int Cell_Height = 26;
const int Level_X_Offset = 23;
const int Level_Y_Offset = 13;
const int Circle_Size = 20;

int Inner_Width = 40;

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
void Init()
//	Функция инициализации |=|=|=| Настройка игры при старте
{
	Create_Pen_Brush(112, 146, 190, Blue_Brick_Pen, Blue_Brick_Brush);
	Create_Pen_Brush(255, 182, 89, Purple_Brick_Pen, Purple_Brick_Brush);
	Create_Pen_Brush(155, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	Create_Pen_Brush(249, 100, 0, Platform_Inner_Pen, Platform_Inner_Brush);

	Arc_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
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
	// 1. Рисуем боковые шарики
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + Circle_Size* Global_Scale, y + Circle_Size * Global_Scale);
	Ellipse(hdc, x + (Circle_Size+Inner_Width), y, x + ((Circle_Size*2)+Inner_Width), y+ Circle_Size);

	// 2. Рисуем среднюю часть
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y+Global_Scale*2, 10 * Global_Scale, 32 * Global_Scale);
						//x+ Inner_Width Расстояние между шариками, может увеличиваться!!!

	// 3. Рисуем среднюю часть
	SelectObject(hdc, Arc_Pen);
	Arc	(hdc, 363, 563, 353, 552, 360, 489, 179, 489 );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
//	Отрисовка экрана игры
{
	Draw_Level(hdc);

	Draw_Platform(hdc, 350, 550);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
