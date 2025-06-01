#include "Engine.h"
#define _USE_MATH_DEFINES
#include "math.h"

enum Ebrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue,
};

HPEN Purple_Brick_Pen, Blue_Brick_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Arc_Pen;
HBRUSH Purple_Brick_Brush, Blue_Brick_Brush, Platform_Circle_Brush, Platform_Inner_Brush, Arc_Brush;

const int Global_Scale = 1;
const int Brick_Width = 61;
const int Brick_Height = 23;
const int Cell_Width = 64;
const int Cell_Height = 26;
const int Level_X_Offset = 23;
const int Level_Y_Offset = 13;
const int Circle_Size = 20;
const int Volume_Rectangle = 59;

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
{// ������� ���������� ����� RGB ����� �������
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Init()
//	������� ������������� |=|=|=| ��������� ���� ��� ������
{
	Create_Pen_Brush(112, 146, 190, Blue_Brick_Pen, Blue_Brick_Brush);
	Create_Pen_Brush(255, 182, 89, Purple_Brick_Pen, Purple_Brick_Brush);
	Create_Pen_Brush(155, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	Create_Pen_Brush(249, 100, 0, Platform_Inner_Pen, Platform_Inner_Brush);
	Create_Pen_Brush(255, 255, 255, Arc_Pen, Arc_Brush);
	}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type)
//	����� �������
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_None:	return;

	case EBT_Purple:
		pen = Purple_Brick_Pen;								 // ������� ���� ��� 2-��� �������
		brush = Purple_Brick_Brush;

		break;

	case EBT_Blue:
		pen = Blue_Brick_Pen;								 // ������� ���� ��� 1-��� �������
		brush = Blue_Brick_Brush;

		break;

	default: return;
	}
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * Global_Scale, y * Global_Scale, Brick_Width + x * Global_Scale, Brick_Height + y * Global_Scale, 10*Global_Scale, 32*Global_Scale);	

	// ������ - ������
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick_Letter(HDC hdc, int x, int y, int rotation_step)
//	��������� ��������� ������� � ������
{
	double offset;
	double rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;		// �������������� ���� � ���� ��������
	int brick_half_height = (Brick_Height / 2);
	int back_part_offset;
	XFORM xForm, old_xForm;

	if (rotation_step == 4 || rotation_step == 12)
	{
		// ������� ���
		SelectObject(hdc, Purple_Brick_Pen);
		SelectObject(hdc, Purple_Brick_Brush);

		Rectangle(hdc, x, y + brick_half_height - 3, x + Volume_Rectangle, y + brick_half_height);

		// ������� �������� ����
		SelectObject(hdc, Blue_Brick_Pen);
		SelectObject(hdc, Blue_Brick_Brush);

		Rectangle(hdc, x, y + brick_half_height , x + Volume_Rectangle, y + brick_half_height + 3);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);

		// ����������� ������� "����������" �����
		xForm.eM11 = 1.0f;
		xForm.eM12 = 0.0f;
		xForm.eM21 = 0.0f;
		xForm.eM22 = (float)cos(rotation_angle);
		xForm.eDx  = (float)x;
		xForm.eDy  = (float)y + (float)brick_half_height;
		GetWorldTransform(hdc, &old_xForm);
		SetWorldTransform(hdc, &xForm);

	 //	������� ���
		SelectObject(hdc, Blue_Brick_Pen);
		SelectObject(hdc, Blue_Brick_Brush);

		offset = (1.0 - fabs(xForm.eM22)) * 12;
		back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Volume_Rectangle, brick_half_height - back_part_offset);

	// ������� �������� ����
		SelectObject(hdc, Purple_Brick_Pen);
		SelectObject(hdc, Purple_Brick_Brush);

		Rectangle(hdc, 0, -brick_half_height, Volume_Rectangle, brick_half_height);

		SetWorldTransform(hdc, &old_xForm);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
//	����� ���� �������� ������
{
	int i,j;

	for (i = 0; i < 14; i++)	
		for (j = 0; j< 12; j++) 
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (Ebrick_Type)Level_01[i][j]);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
//	������ ���������
{
	// 1. ������ ������� ������
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);
	Ellipse(hdc, x, y, x + Circle_Size* Global_Scale, y + Circle_Size * Global_Scale);
	Ellipse(hdc, x + (Circle_Size+Inner_Width), y, x + ((Circle_Size*2)+Inner_Width), y+ Circle_Size);

	// 2. ������ ������� �����
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);
	RoundRect(hdc, x+9, y+18, x + (Inner_Width + 31), y+Global_Scale*2, 10 * Global_Scale, 32 * Global_Scale);
						//x + Inner_Width ���������� ����� ��������, ����� �������������!!!

	// 3. ������ ������� �����
	SelectObject(hdc, Arc_Pen);
	Arc	(hdc, 363, 563, 353, 552, 360, 489, 179, 489 );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
//	��������� ������ ����
{
	/*Draw_Level(hdc);

	Draw_Platform(hdc, 350, 550);*/

	int i;
	for (i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 200 + i * Brick_Width, 200, i);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
