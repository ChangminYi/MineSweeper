//승패 결정하는 함수

#include <stdio.h>
#include <stdlib.h>

#define MINE 1
#define TRUE 1
#define FALSE 0

typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
}mine;

int winlose(mine **arg, int row, int col);

int winlose(mine **arg, int row, int col)
{
	if (arg[row][col].stat == MINE)
	{
		printf("You Lose!\n");
		return 0;
	}
	else if (arg[row][col].stat != MINE)
	{
		arg[row][col].open = TRUE;
		return 1;
	}
}