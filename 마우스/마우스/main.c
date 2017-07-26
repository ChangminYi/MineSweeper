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
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);		//�ܼ�â �Է� ����

	if (rec.EventType == MOUSE_EVENT)		//���콺 �̺�Ʈ
	{
		if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)		//����Ŭ��
		{
			int mouse_x = rec.Event.MouseEvent.dwMousePosition.X;		//X�� ����
			int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y;		//Y�� ����
			COORD Coor = { 0, 0 };
			DWORD dw;

			FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Coor, &dw);		//�ܼ� ȭ�� ����
			//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor);		//Ŀ�� Coor�� �̵���Ŵ
			printf("%d, %d", mouse_x, mouse_y);
			return;
		}
	}
}
