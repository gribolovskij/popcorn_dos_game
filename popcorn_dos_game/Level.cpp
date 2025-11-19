#include "Level.h"
#include "Config.h"

//-------------------AsLevel
// 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char AsLevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
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
char AsLevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
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
AsLevel::AsLevel()
	:	Level_Rect{}, paint_area{}, Current_Brick_Left_X(0),
	Current_Brick_Right_X(0), Current_Brick_Y_High(0), Current_Brick_Y_Low(0), Current_Level{}, Action_Brick_Count(), Falling_Brick_Count(), Falling_Letter{}, Action_Brick{}
{
} 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Init()
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
void AsLevel::Act()
{
	ObjectsToAct(AsConfig::Max_Action_Brick_Count, (AGraphics_Object **)&Action_Brick);
	ObjectsToAct(AsConfig::Max_Falling_Letter_Count, (AGraphics_Object **)&Falling_Letter);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::ObjectsToAct(int object_max_count, AGraphics_Object **object_array)
{
	int i;
	for (i = 0; i < object_max_count; i++)
	{
		if (object_array[i] != 0)
		{
			object_array[i]->Act();

			if (object_array[i]->Is_Finished())
			{
				delete object_array[i];
				object_array[i] = 0;
				--object_array;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT &paint_area)
{
	int i, j;
	RECT intersection_rect, brick_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
	{

		for (i = 0; i < AsConfig::Level_Height; i++)
			for (j = 0; j < AsConfig::Level_Width; j++)
			{
					brick_rect.left = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
					brick_rect.top = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
					brick_rect.right = brick_rect.left + AsConfig::Brick_Width;
					brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height;

					if (IntersectRect(&intersection_rect, &paint_area, &brick_rect) )
				Draw_Brick(hdc, brick_rect, (EBrick_Type)Current_Level[i][j]);
			}

		Draw_Graphics_Objects(hdc, paint_area, (AGraphics_Object**)&Action_Brick , AsConfig::Max_Action_Brick_Count);
	}
				Draw_Graphics_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letter , AsConfig::Max_Falling_Letter_Count);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Get_Falling_Letter(int &index, AFalling_Letter **falling_letter)
{
	AFalling_Letter *current_letter;

	if (Falling_Brick_Count == 0)
		return false;

	if (index < 0 || index >= AsConfig::Max_Falling_Letter_Count)
		return false;

	while (index < AsConfig::Max_Falling_Letter_Count)
	{
		current_letter = Falling_Letter[index++];

		if (current_letter != 0)
		{
			*falling_letter = current_letter;
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Graphics_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array ,int object_max_count)
{
	int i;
	for (i = 0; i < object_max_count; i++)
	{
		if (objects_array[i] != 0)
			objects_array[i]->Draw(hdc, paint_area);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
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
	RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right - 1, brick_rect.bottom - 1, 10, 32);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::Set_Current_Level(char Level[AsConfig::Level_Height][AsConfig::Level_Width])
{
	memcpy(Current_Level, Level, sizeof(Current_Level) );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsLevel::On_Hit(int brick_x, int brick_y)
{
	EBrick_Type brick_type;
	
	brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

	if (Add_Falling_Letter(brick_x, brick_y, brick_type))
		Current_Level[brick_y][brick_x] = EBT_None;
	else
		Add_Active_Brick(brick_x, brick_y, brick_type);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
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
						letter_x = brick_x * AsConfig::Cell_Width + AsConfig::Level_X_Offset;
						letter_y = brick_y * AsConfig::Cell_Height + AsConfig::Level_Y_Offset;

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
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
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
void AsLevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
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
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflect_pos)
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
bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double& reflect_pos)
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
