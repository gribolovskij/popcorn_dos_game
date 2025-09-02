#include "Level.h"
#include "Config.h"
// ALevel
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
	0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
	0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char ALevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
	: Level_Rect{}, Purple_Brick_Pen(0), Blue_Brick_Pen(0), Letter_Pen(0), Purple_Brick_Brush(0), Blue_Brick_Brush(0), paint_area{}
{
} 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	// Correction position when reflecting from the bricks
	int i, j;

	double direction;
	double min_ball_x, max_ball_x;
	double min_ball_y, max_ball_y;
	int min_level_x, max_level_x;
	int min_level_y, max_level_y;
	bool horizon_got_hit, vertical_got_hit;
	double horizon_reflect_pos, vertical_reflect_pos;

	if (next_y_pos + ball->Radius > AsConfig::Level_Y_Offset + (AsConfig::Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height)
		return false;

	direction = ball -> Get_Direction();

	min_ball_x = next_x_pos - ball -> Radius;
	max_ball_x = next_x_pos + ball -> Radius;
	min_ball_y = next_y_pos - ball -> Radius;
	max_ball_y = next_y_pos + ball -> Radius;

	min_level_x = (int)(min_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width;
	max_level_x = (int)(max_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width;
	min_level_y = (int)(min_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height;
	max_level_y = (int)(max_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height;

	for (i = max_level_y; i >= min_level_y; i--)
	{
		Current_Brick_Y_High = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
		Current_Brick_Y_Low =  Current_Brick_Y_High + AsConfig::Brick_Height;

		for (j = min_level_x; j <= max_level_x; j++)
		{
			if (Current_Level[i][j] == 0)
				continue;

			Current_Brick_Left_X = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
			Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

			horizon_got_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizon_reflect_pos);

			vertical_got_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflect_pos);

			if (horizon_got_hit && vertical_got_hit)
			{
				if (vertical_reflect_pos < horizon_reflect_pos)
					ball -> Reflect(true);
				else
					ball -> Reflect(false);
				return true;
			}
			else
				if (horizon_got_hit)
				{
					ball -> Reflect(false);
					return true;
				}
				else
					if (vertical_got_hit)
					{
						ball -> Reflect(true);
						return true;
					}
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(AsConfig::Gray_Brick_Color, Blue_Brick_Pen, Blue_Brick_Brush);
	AsConfig::Create_Pen_Brush(AsConfig::Orange_Brick_Color, Purple_Brick_Pen, Purple_Brick_Brush);

	Level_Rect.left = AsConfig::Level_X_Offset;
	Level_Rect.top = AsConfig::Level_Y_Offset;
	Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width;
	Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height;

	memset(Current_Level, 0, sizeof(Current_Level) );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos)
{
	double direction = ball -> Get_Direction(); 

	// Check hit from down .. Checking for hits on the lower edge
	if (direction >= 0 && direction < M_PI)

		if (Hit_Circle_Line(next_y_pos - Current_Brick_Y_Low, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, next_x_pos, reflect_pos))
		{
			// Ñhecking the possibility of a downward rebound
			if (level_y < AsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0 )
			{
			return true;
			}
			else
				return false;
		}

	// Check hit from up .. Checking for hits on the higher edge
	if (direction >= M_PI && direction <= 2.0 * M_PI)

		if (Hit_Circle_Line(next_y_pos - Current_Brick_Y_High, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, next_x_pos, reflect_pos))
		{
			// Ñhecking the possibility of a upward rebound
			if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0 )
			{
			return true;
			}
			else
			return false;
		}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos)
{
	double direction = ball -> Get_Direction();

	// Check hit from left edge.. Checking for hits on the left edge
	if (direction >= 0.0 && direction < M_PI_2 || direction >= M_PI + M_PI_2 && direction <= 2.0 * M_PI)

		if (Hit_Circle_Line(Current_Brick_Left_X - next_x_pos, Current_Brick_Y_High, Current_Brick_Y_Low, ball->Radius, next_y_pos, reflect_pos))
		{
			// Ñhecking the possibility of a leftward rebound
			if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0 )
			{
			return true;
			}
			else 
				return false;
		}

	// Check hit from right edge.. Checking for hits on the right edge
	if (direction > M_PI_2 && direction < M_PI + M_PI_2)

		if (Hit_Circle_Line(Current_Brick_Right_X - next_x_pos, Current_Brick_Y_High, Current_Brick_Y_Low, ball->Radius, next_y_pos, reflect_pos))
		{
			// Ñhecking the possibility of a rightward rebound
			if (level_x < AsConfig::Level_Width - 1 && Current_Level[level_y][level_x + 1] == 0 )
			{
			return true;
			}
			else
				return false;
		}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Current_Level(char Level[AsConfig::Level_Height][AsConfig::Level_Width])
{
	memcpy(Current_Level, Level, sizeof(Current_Level) );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
		return;

	int i,j;

	for (i = 0; i < AsConfig::Level_Height; i++)
		for (j = 0; j< AsConfig::Level_Width; j++)
			Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, (Ebrick_Type)Current_Level[i][j]);

	//Action_Brick.Draw(hdc, paint_area);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Hit_Circle_Line(double y, double left_x, double right_x, double radius, double next_x_pos, double &x)
{
	// Check intersection horizontal segment (START left_x FINISH right_x) with a circle of radius

	double min_x, max_x;

	// x * x + y * y = R * R
	// x = sqrt(R * R - y * y)
	// y = sqrt(R * R - x * x)

	if (y >  radius)
		return false;

	x = sqrt(radius * radius - y * y);

	max_x = next_x_pos + x;
	min_x = next_x_pos - x;

	if (max_x >= left_x && max_x <= right_x || min_x >= left_x && min_x <= right_x)

		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, Ebrick_Type brick_type)
//	Âûâîä êèðïè÷à
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_None:	return;

	case EBT_Purple:
		pen = Purple_Brick_Pen;								 // Create color for 2 brick
		brush = Purple_Brick_Brush;

		break;

	case EBT_Blue:
		pen = Blue_Brick_Pen;								 // Create color for 1 brick
		brush = Blue_Brick_Brush;

		break;

	default: return;
	}
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, AsConfig::Brick_Width + x * AsConfig::Global_Scale, AsConfig::Brick_Height + y * AsConfig::Global_Scale, 10*AsConfig::Global_Scale, 32*AsConfig::Global_Scale);

	// Brick - paint
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)	
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
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, Ebrick_Type brick_type, ELetter_Type letter_type,int rotation_step)
//	Drawing Falling Break with Letter
{
	bool switch_color;
	double offset;
	double rotation_angle;		// Converting step to rotation angle
	int brick_half_height = (AsConfig::Brick_Height / 2);
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xForm, old_xForm;

	if (!(brick_type == EBT_Blue || brick_type == EBT_Purple))
		return;		// Falling letters can only be from this type of brick

	// Adjust the rotation step and rotation angle
	rotation_step = rotation_step % 16;											// Take the remainder of the division by 16 and put it back into the variable

	if (rotation_step < 8)
		rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;				// Delayed initialization
	else
		rotation_angle = 2.0 * M_PI / 16 * (double)(8L - rotation_step);

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
		// Display the background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - 3, x + AsConfig::Volume_Rectangle, y + brick_half_height);

		// Display the foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height , x + AsConfig::Volume_Rectangle, y + brick_half_height + 3);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);

		// Set up the letter "flip" matrix
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
		RoundRect(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Volume_Rectangle, brick_half_height - back_part_offset, 10, 32);

		// Deduced foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		RoundRect(hdc, 0, brick_half_height, AsConfig::Volume_Rectangle, -brick_half_height, 10, 32);

		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELT_O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, AsConfig::Circle_Size, -AsConfig::Y_Letter, AsConfig::Circle_Size * 2, AsConfig::Y_Letter);
			}
		}
		SetWorldTransform(hdc, &old_xForm);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
