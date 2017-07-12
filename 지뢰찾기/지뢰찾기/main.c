#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINE 1
#define NOT 0
#define TRUE 1
#define FALSE 0
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
int mine_seed(mine **arg, int row, int col);
int mine_check(mine **arg, int row, int col);
void first_select(mine **arg);
int select(mine **arg);
void screen(mine **arg);

//Global Variables
int row = 0, col = 0;

void main()
{
	//초기 설정
	mine** memloc = setting();

	mine_seed(memloc, row, col);
	mine_check(memloc, row, col);

	//게임 진행
	screen(memloc);
	first_select(memloc);
	do
	{
		screen(memloc);
	} while (select(memloc) == 1);

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
	mine **col_p = (mine **)calloc(row, sizeof(mine *));
	for (int i = 0; i < row; i++)
	{
		col_p[i] = (mine *)calloc(col, sizeof(mine));
		for (int r = 0; r < col; r++)
		{
			col_p[i][r].num = col*i + r + 1;
		}
	}

	//recording memory location
	return col_p;
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
	int open_row = 0, open_col = 0;

	printf("열 칸 입력하세요(행): ");
	scanf_s("%d", &open_row);
	open_row--;
	printf("열 행 입력하세요(열): ");
	scanf_s("%d", &open_col);
	open_col--;

	if (arg[open_row][open_col].stat == MINE)
	{
		printf("You Lose!\n");
		return 0;
	}
	else
	{
		arg[open_row][open_col].open = TRUE;
		return 1;
	}
}

void screen(mine ** arg)
{
	system("cls");

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (arg[i][j].open == TRUE && arg[i][j].peri != 0)
			{
				printf("    %d", arg[i][j].peri);
			}
			if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
			{
				printf("     ");
			}
			else if (arg[i][j].open == FALSE)
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

void first_select(mine ** arg)
{
	int open_row = 0, open_col = 0;

	printf("열 칸 입력하세요(행): ");
	scanf_s("%d", &open_row);
	open_row--;
	printf("열 행 입력하세요(열): ");
	scanf_s("%d", &open_col);
	open_col--;

	if (arg[open_row][open_col].stat == MINE)
	{
		do
		{
			mine_seed(arg, row, col);
			mine_check(arg, row, col);
		} while (arg[open_row][open_col].stat == MINE);
	}

	arg[open_row][open_col].open = TRUE;
}
