#include <stdio.h>
#include <Windows.h>

INPUT_RECORD rec;
DWORD dwNOER;

void checkMouse();

void main()
{
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	while (1)
	{
		checkMouse();
	}
}

void checkMouse()
{
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);		//콘솔창 입력 받음

	if (rec.EventType == MOUSE_EVENT)		//마우스 이벤트
	{
		if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)		//왼쪽클릭
		{
			int mouse_x = rec.Event.MouseEvent.dwMousePosition.X;		//X값 받음
			int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y;		//Y값 받음
			COORD Coor = { 0, 0 };
			DWORD dw;

			FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Coor, &dw);		//콘솔 화면 지움
			//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor);		//커서 Coor로 이동시킴
			printf("%d, %d", mouse_x, mouse_y);
			return;
		}
	}
}
