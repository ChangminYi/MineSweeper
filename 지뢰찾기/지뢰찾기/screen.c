//지뢰찾기 판 출력 함수

#include <stdio.h>

#define MINE 1
#define TRUE 1
#define FALSE 0
#define MINE_SEL 2

typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
	int sel;
}mine;

void screen(mine ** arg);

void screen(mine ** arg)
{
	extern row, col;

	//출력용 char
	unsigned char close = 'O';
	char open = ' ';
	char mine = 9;
	char mine_checked = 'X';

	system("cls");

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (arg[i][j].open == TRUE && arg[i][j].peri != 0)
			{
				printf("%5d", arg[i][j].peri);
			}
			else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
			{
				printf("%5c", open);
			}
			else if (arg[i][j].open == FALSE && arg[i][j].stat == MINE_SEL)
			{
				printf("%5c", mine_checked);
			}
			else if (arg[i][j].open == FALSE)
			{
				printf("%5c", close);
			}
		}
		printf("\n\n");
	}
}