#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned  char r, unsigned  char g, unsigned  char b, HPEN &pen, HBRUSH &brush)
{// Функция реализации цвета RGB одной строкой //	Function to implement RGB color in one line
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
