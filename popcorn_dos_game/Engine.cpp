#include "Engine.h"

enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};

HPEN Purple_Brick_Pen, Blue_Brick_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
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
void Init()
//	Функция инициализации |=|=|=| Настройка игры при старте
{
	Blue_Brick_Pen = CreatePen(PS_SOLID, 0, RGB(112, 146, 190));								 // Создаем цвет для 1-ого кирпича
	Blue_Brick_Brush = CreateSolidBrush(RGB(112, 146, 190));

	Purple_Brick_Pen = CreatePen(PS_SOLID, 0, RGB(255, 182, 89));								 // Создаем цвет для 2-ого кирпича
	Purple_Brick_Brush = CreateSolidBrush(RGB(255, 182, 89));

	Platform_Circle_Pen = CreatePen(PS_SOLID, 0, RGB(155, 0, 0));
	Platform_Circle_Brush = CreateSolidBrush(RGB(155, 0, 0));

	Platform_Inner_Pen = CreatePen(PS_SOLID, 0, RGB(249, 100, 0));
	Platform_Inner_Brush = CreateSolidBrush(RGB(249, 100, 0));
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
	// Красим шарики платформы

	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + Circle_Size* Global_Scale, y + Circle_Size * Global_Scale);
	Ellipse(hdc, x + (Circle_Size+Inner_Width), y, x + ((Circle_Size*2)+Inner_Width), y+ Circle_Size);

	// Красим середину платформы

	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y+Global_Scale*2, 10 * Global_Scale, 32 * Global_Scale);
						//x+ Inner_Width Расстояние между шариками, может увеличиваться!!!
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
//	Отрисовка экрана игры
{
	Draw_Level(hdc);

	Draw_Platform(hdc, 350, 550);

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
