#include <stdio.h>
#include <Windows.h>

#define TRUE 1

INPUT_RECORD rec;
DWORD dwNOER;

int mouse_x = 0, mouse_y = 0;

void checkMouse();

void checkMouse()
{
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	while (TRUE)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);

		if (rec.EventType == MOUSE_EVENT)
		{				
			COORD Coor = { 0,0 };
			DWORD dw;
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				mouse_x = rec.Event.MouseEvent.dwMousePosition.X;
				mouse_y = rec.Event.MouseEvent.dwMousePosition.Y;

				FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Coor, &dw);
				return;
			}
		}
	}
}