#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINE 1
#define NOT 0
#define mine_plus arg[i][j].peri++;

//Mine Structure
typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
}mine;

//Functions
mine ** setting();
int mine_seed(mine ** arg, int row, int col);
int mine_check(mine ** arg, int row, int col);
int select(mine ** arg);
void screen(mine **arg);

//Global Variables
int row = 0, col = 0;
mine** memloc = NULL;

void main()
{
	setting();
	mine_seed(memloc, row, col);
	mine_check(memloc, row, col);

	screen(memloc);

	printf("\n\n");
	system("pause");

	return;
}

mine ** setting()
{
	//size confirming...
	printf("난이도 선택\n");
	printf("가로줄 갯수: ");
	scanf_s("%d", &row);
	printf("세로줄 갯수: ");
	scanf_s("%d", &col);

	//memory allocating & initializing...
	mine **po_col = (mine **)calloc(row, sizeof(mine *));
	for (int i = 0; i < row; i++)
	{
		po_col[i] = (mine *)calloc(col, sizeof(mine));
		for (int r = 0; r < col; r++)
		{
			po_col[i][r].num = col*i + r + 1;
		}
	}

	//recording memory location
	memloc = po_col;
}

int mine_seed(mine ** argv, int row, int col)
{
	int *mine = NULL;
	srand((unsigned)time(NULL));

	//size defining
	int mi_size = 0;

	//Figuring the number of mines
	if (row*col <= 30)
	{
		mine = (int)calloc(8, sizeof(int));
		mi_size = 7;
	}
	else if (row*col <= 60)
	{
		mine = (int)calloc(15, sizeof(int));
		mi_size = 12;
	}
	else if (row*col <= 120)
	{
		mine = (int)calloc(25, sizeof(int));
		mi_size = 20;
	}
	else if (row*col <= 180)
	{
		mine = (int)calloc(50, sizeof(int));
		mi_size = 40;
	}
	else
	{
		mine = (int)calloc(75, sizeof(int));
		mi_size = 60;
	}

	//seeding mine
	for (int i = 0; i < mi_size; i++)
	{
		mine[i] = rand() % (row*col - 1) + 1;
	}
	

	//checking multi-selected number
	int temp = 0;

	for (int i = 0; i < mi_size; i++)
	{
		temp = mine[i];
		for (int j = 0; j < mi_size; j++)
		{
			if (i == j)
			{
				break;
			}
			if (mine[j] == temp)
			{
				mine[j] = rand() % (row*col - 1) + 1;
			}
			else
			{
				continue;
			}
		}
	}

	//applying to mine_stat
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			for (int k = 0; k < mi_size; k++)
			{
				if (argv[i][j].num == mine[k])
				{
					argv[i][j].stat = MINE;
					break;
				}
				else
				{
					argv[i][j].stat = NOT;
				}
			}
		}
	}

	free(mine);
	return 0;
}

int select(mine ** arg)
{
	return 0;
}

void screen(mine ** arg)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (arg[i][j].stat == MINE)
			{
				printf("    X");
			}
			else
			{
				printf("    O");
			}
		}
		printf("\n\n");
	}
}

int mine_check(mine ** arg, int row, int col)
{
	//i=row, j=column
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			arg[i][j].peri = 0;
			if (i == 0 && j == 0)
			{
				if (arg[i][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j + 1].stat == MINE)
				{
					mine_plus
				}
			}
			else if (i == 0 && j == col - 1)
			{
				if (arg[i][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j - 1].stat == MINE)
				{
					mine_plus
				}
			}
			else if (i == row - 1 && j == 0)
			{
				if (arg[i][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j + 1].stat == MINE)
				{
					mine_plus
				}
			}
			else if (i == row - 1 && j == col - 1)
			{
				if (arg[i - 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j - 1].stat == MINE)
				{
					mine_plus
				}
			}
			else if (i == 0)
			{
				if (arg[i][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j + 1].stat == MINE)
				{
					mine_plus
				}
			}
			else if (i == row - 1)
			{
				if (arg[i][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j + 1].stat == MINE)
				{
					mine_plus
				}
			}
			else if (j == 0)
			{
				if (arg[i - 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j].stat == MINE)
				{
					mine_plus
				}
			}
			else if (j == col - 1)
			{
				if (arg[i - 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j].stat == MINE)
				{
					mine_plus
				}
			}
			else
			{
				if (arg[i - 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j - 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j].stat == MINE)
				{
					mine_plus
				}
				if (arg[i + 1][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i][j + 1].stat == MINE)
				{
					mine_plus
				}
				if (arg[i - 1][j + 1].stat == MINE)
				{
					mine_plus
				}
			}
		}
	}
	return 0;
}