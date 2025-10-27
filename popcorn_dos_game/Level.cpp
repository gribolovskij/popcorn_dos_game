#include "Level.h"
#include "Config.h"

// AFalling_Letter
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
	: Brick_Type(brick_type), Letter_Type(letter_type), X(x), Y(y), Got_Hit(false), Rotation_Step(2), Next_Rotation_Tick(AsConfig::Tick_Current_Timer + Tick_Per_Step)
{
	Letter_Cell.left = X;
	Letter_Cell.top = Y;
	Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width;
	Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height;

	Prev_Letter_Cell = Letter_Cell;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
	Prev_Letter_Cell = Letter_Cell;

	Y += 5;
	Letter_Cell.top += 5;
	Letter_Cell.bottom += 5;


	if (AsConfig::Tick_Current_Timer >= Next_Rotation_Tick)
	{
		++Rotation_Step;
		Next_Rotation_Tick += Tick_Per_Step;
	}

	InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
	{
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);

		Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
	Draw_Brick_Letter(hdc);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
	if (Got_Hit || Letter_Cell.top >= AsConfig::Max_Y_Pos)

		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
//	Drawing Falling Break with Letter
{
	bool switch_color;
	double offset;
	// Converting step to rotation angle
	double rotation_angle;
	int brick_half_height = (AsConfig::Brick_Height / 2);
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xForm, old_xForm;

	if (!( Brick_Type == EBT_Blue || Brick_Type == EBT_Yellow))
		// Falling letters can only be from this type of brick
		return;

	// Adjust the rotation step and rotation angle
	// Take the remainder of the division by 16 and put it back into the variable
	Rotation_Step = Rotation_Step % 16;

	if (Rotation_Step < 8)
		// Delayed initialization
		rotation_angle = 2.0 * M_PI / 16.0 * (double)Rotation_Step;
	else
		rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - Rotation_Step);

	if (Rotation_Step > 4 && Rotation_Step <= 12)
	{
		if (Brick_Type == EBT_Blue)
			switch_color = true;
		else
			switch_color = false;
		//	switch_color = brick_type == EBT_Blue;
	}
	else
	{
		if (Brick_Type == EBT_Yellow)
			switch_color = true;
		else
			switch_color = false;
	}
	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (Rotation_Step == 4 || Rotation_Step == 12)
	{
		// Display the background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, X, Y + brick_half_height - 3, X + AsConfig::Volume_Rectangle, Y + brick_half_height);

		// Display the foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, X, Y + brick_half_height, X + AsConfig::Volume_Rectangle, Y + brick_half_height + 3);
	}
	else
	{
		// Set up the letter "flip" matrix
		xForm.eM11 = 1.0f;
		xForm.eM12 = 0.0f;
		xForm.eM21 = 0.0f;
		xForm.eM22 = (float)cos(rotation_angle);
		xForm.eDx = (float)X;
		xForm.eDy = (float)Y + (float)brick_half_height;

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

		if (Rotation_Step > 4 && Rotation_Step <= 12)
		{
			if (Letter_Type == ELT_O)
			{
				SelectObject(hdc,  AsConfig::Letter_Pen);
				Ellipse(hdc, AsConfig::Circle_Size, -AsConfig::Y_Letter, AsConfig::Circle_Size * 2, AsConfig::Y_Letter);
			}
		}
		SetWorldTransform(hdc, &old_xForm);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
	if (is_switch_color)
	{
		front_pen = AsConfig::Blue_Brick_Pen;
		front_brush = AsConfig::Blue_Brick_Brush;
		back_pen = AsConfig::Purple_Brick_Pen;
		back_brush = AsConfig::Purple_Brick_Brush;
	}
	else
	{
		front_pen = AsConfig::Purple_Brick_Pen;
		front_brush = AsConfig::Purple_Brick_Brush;
		back_pen = AsConfig::Blue_Brick_Pen;
		back_brush = AsConfig::Blue_Brick_Brush;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// ALevel
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
	1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
	:	Level_Rect{}, paint_area{}, Current_Brick_Left_X(0),
	Current_Brick_Right_X(0), Current_Brick_Y_High(0), Current_Brick_Y_Low(0), Current_Level{}, Action_Brick_Count(), Falling_Brick_Count(), Falling_Letter{}, Action_Brick{}
{
} 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
	Level_Rect.left = AsConfig::Level_X_Offset;
	Level_Rect.top = AsConfig::Level_Y_Offset;
	Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width;
	Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height;

	memset(Current_Level, 0, sizeof(Current_Level) );
	memset(Action_Brick, 0, sizeof(Action_Brick) );
	memset(Falling_Letter, 0, sizeof(Falling_Letter));
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Act()
{
	int i;

	for (i = 0; i < AsConfig::Max_Action_Brick_Count; i++)
	{
		if (Action_Brick[i] != 0)
		{
			Action_Brick[i]->Act();

			if (Action_Brick[i]->Is_Finished() )
			{
				delete Action_Brick[i];
				Action_Brick[i] = 0;
				--Action_Brick_Count;
			}
		}
	}

	//!!! Copy Logic
// Encapsulated logic for animating level elements (active bricks)
	for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
	{
		if (Falling_Letter[i] != 0)
		{
			Falling_Letter[i]->Act();

			if (Falling_Letter[i]->Is_Finished())
			{
				delete Falling_Letter[i];
				Falling_Letter[i] = 0;
				--Falling_Brick_Count;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j;
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{

		for (i = 0; i < AsConfig::Level_Height; i++)
			for (j = 0; j < AsConfig::Level_Width; j++)
				Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, (EBrick_Type)Current_Level[i][j]);

		for (i = 0; i < AsConfig::Max_Action_Brick_Count; i++)
		{
			if (Action_Brick[i] != 0)
				Action_Brick[i]->Draw(hdc, paint_area);
		}
	}

	//!!! Copy Logic
	for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
	{
		if (Falling_Letter[i] != 0)
			Falling_Letter[i]->Draw(hdc, paint_area);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Current_Level(char Level[AsConfig::Level_Height][AsConfig::Level_Width])
{
	memcpy(Current_Level, Level, sizeof(Current_Level) );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::On_Hit(int brick_x, int brick_y)
{
	EBrick_Type brick_type;
	
	brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

	if (Add_Falling_Letter(brick_x, brick_y, brick_type))
		Current_Level[brick_y][brick_x] = EBT_None;
	else
		Add_Active_Brick(brick_x, brick_y, brick_type);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{
	// Create Falling Letter
	int i;
	int letter_x, letter_y;

	AFalling_Letter *falling_letter;

	if (brick_type == EBT_Yellow || brick_type == EBT_Blue)
	{
		if (AsConfig::Rand(AsConfig::Hits_Per_Letter) == 0)
		{
			if (Falling_Brick_Count < AsConfig::Max_Falling_Letter_Count)
			{
				for (i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
				{
					if (Falling_Letter[i] == 0)
					{
						letter_y = brick_y * AsConfig::Cell_Height + AsConfig::Level_Y_Offset;
						letter_x = brick_x * AsConfig::Cell_Width + AsConfig::Level_X_Offset;

						falling_letter = new AFalling_Letter(brick_type, ELT_O, letter_x, letter_y);

						Falling_Letter[i] = falling_letter;
						++Falling_Brick_Count;
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
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

	direction = ball->Get_Direction();

	min_ball_x = next_x_pos - ball->Radius;
	max_ball_x = next_x_pos + ball->Radius;
	min_ball_y = next_y_pos - ball->Radius;
	max_ball_y = next_y_pos + ball->Radius;

	min_level_x = (int)(min_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width;
	max_level_x = (int)(max_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width;
	min_level_y = (int)(min_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height;
	max_level_y = (int)(max_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height;

	for (i = max_level_y; i >= min_level_y; i--)
	{
		Current_Brick_Y_High = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
		Current_Brick_Y_Low = Current_Brick_Y_High + AsConfig::Brick_Height;

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
					ball->Reflect(true);
				else
					ball->Reflect(false);
				On_Hit(j, i);

				return true;
			}
			else
				if (horizon_got_hit)
				{
					ball->Reflect(false);
					On_Hit(j, i);
					return true;
				}
				else
					if (vertical_got_hit)
					{
						ball->Reflect(true);
						On_Hit(j, i);
						return true;
					}
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
{
	//Create Active_Brick
	int i;
	AAction_Brick* action_brick;

	if (Action_Brick_Count >= AsConfig::Max_Action_Brick_Count)
		return;

	switch (brick_type)
	{
	case EBT_None:
		return;

	case EBT_Blue:
	case EBT_Yellow:
		action_brick = new AAction_Brick(brick_type, brick_x, brick_y);
		Current_Level[brick_y][brick_x] = EBT_None;
		break;

	default:
		return;
	}

	// Add new action_brick to the first available seat.

	for (i = 0; i < AsConfig::Max_Action_Brick_Count; i++)
	{
		if (Action_Brick[i] == 0)
		{
			Action_Brick[i] = action_brick;
			++Action_Brick_Count;
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos)
{
	double direction = ball -> Get_Direction(); 

	// Check hit from down..Checking for hits on the lower edge
	if (ball->Is_Moving_Up())
	{
		if (Hit_Circle_Line(next_y_pos - Current_Brick_Y_Low, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, next_x_pos, reflect_pos))
		{// Ñhecking the possibility of a downward rebound
			if (level_y < AsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0)
			{
				return true;
			}
			else
				return false;
		}
	}
	else
	{// Check hit from up .. Checking for hits on the higher edge
		if (Hit_Circle_Line(next_y_pos - Current_Brick_Y_High, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, next_x_pos, reflect_pos))
		{
			if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
			{
				return true;
			}
			else
				return false;
		}
	return false;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double& reflect_pos)
{
	double direction = ball->Get_Direction();

	// Check hit from left edge.. Checking for hits on the left edge
	if ( !ball->Is_Moving_Left())
	{
		if (Hit_Circle_Line(Current_Brick_Left_X - next_x_pos, Current_Brick_Y_High, Current_Brick_Y_Low, ball->Radius, next_y_pos, reflect_pos))
		{
			// Ñhecking the possibility of a leftward rebound
			if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
			{
				return true;
			}
			else
				return false;
		}
}
	else
	{// Check hit from right edge.. Checking for hits on the right edge
		if (Hit_Circle_Line(Current_Brick_Right_X - next_x_pos, Current_Brick_Y_High, Current_Brick_Y_Low, ball->Radius, next_y_pos, reflect_pos))
		{
			// Ñhecking the possibility of a rightward rebound
			if (level_x < AsConfig::Level_Width - 1 && Current_Level[level_y][level_x + 1] == 0)
			{
				return true;
			}
			else
				return false;
		}
		return false;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
//	Âûâîä êèðïè÷à
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_None:
		pen = AsConfig::BG_Pen;
		brush = AsConfig::BG_Brush;
		break;


	case EBT_Yellow:
		pen = AsConfig::Purple_Brick_Pen;
		brush = AsConfig::Purple_Brick_Brush;
		break;

	case EBT_Blue:
		pen = AsConfig::Blue_Brick_Pen;
		brush = AsConfig::Blue_Brick_Brush;
		break;

	default: 
		return;

	}
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x, y, AsConfig::Brick_Width + x - 1, AsConfig::Brick_Height + y - 1, 10, 32);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
