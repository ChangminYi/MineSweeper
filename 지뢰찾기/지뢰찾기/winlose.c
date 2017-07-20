//승패 결정하는 함수

#include <stdio.h>

#define MINE 1
#define TRUE 1
#define FALSE 0

typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
	int sel;
}mine;

int winlose(mine **arg, int row, int col);

int winlose(mine **arg, int row, int col)
{
	extern mi_size, count;
	int all_mine = mi_size;
	int mi_sltd = count;

	if (mi_sltd == all_mine)
	{
		screen(arg);
		printf("You Win!\n");

		return 0;
	}
	else if (arg[row][col].stat == MINE)
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