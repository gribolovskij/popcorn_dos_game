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

	Platform_Circle_Pen = CreatePen(PS_SOLID, 0, RGB(234, 206, 15));
	Platform_Circle_Brush = CreateSolidBrush(RGB(234, 206, 15));

	Platform_Inner_Pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
	Platform_Inner_Brush = CreateSolidBrush(RGB(0, 0, 255));
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
//	
{
	HPEN pen;														// Красим шарики платформы
	HBRUSH brush;

	pen = Platform_Circle_Pen;								
	brush = Platform_Circle_Brush;	
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Ellipse(hdc, x, y, x+20, y+20);
	Ellipse(hdc, x+60, y, x+80, y+20);


	HPEN pen_in;													// Красим середину платформы
	HBRUSH brush_in;

	pen_in = Platform_Inner_Pen;								
	brush_in = Platform_Inner_Brush;	
	SelectObject(hdc, pen_in);
	SelectObject(hdc, brush_in);
	RoundRect(hdc, x+9, y+18, x+70, y+Global_Scale*2, 10 * Global_Scale, 32 * Global_Scale);

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
//	Отрисовка экрана игры
{
	Draw_Level(hdc);

	Draw_Platform(hdc, 350, 550);

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
