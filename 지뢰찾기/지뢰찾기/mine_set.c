//����ã�� �ǿ� ���� �����ϴ� �Լ�

#include <stdio.h>
#include <stdlib.h>

#define MINE 1
#define NOT 0
#define mine_plus arg[i][j].peri++;
#define NOT_SEL 0;

typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
	int sel;
}mine;

mine ** setting();
int mine_seed(mine ** argv, int row, int col);
int mine_check(mine ** arg, int row, int col);

mine ** setting()
{
	extern row, col;

	//����ã�� �� ũ�� ����
	printf("���̵� ����\n");
	printf("������ ����: ");
	scanf_s("%d", &row);
	printf("������ ����: ");
	scanf_s("%d", &col);

	//�޸� �Ҵ� + mine.num �ʱ�ȭ
	mine **col_p = (mine **)calloc(row, sizeof(mine *));
	for (int i = 0; i < row; i++)
	{
		col_p[i] = (mine *)calloc(col, sizeof(mine));
		for (int r = 0; r < col; r++)
		{
			col_p[i][r].num = col*i + r + 1;
			col_p[i][r].sel = NOT_SEL;
			col_p[i][r].open = NOT;
		}
	}

	//�ּҰ� ��ȯ
	return col_p;
}

int mine_seed(mine ** argv, int row, int col)
{
	//���� ����
	extern mi_size;

	//���� ���� ����
	int *mine = NULL;

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

	//���� ������ ĭ �����Լ��� ����
	srand((unsigned)time(NULL));
	for (int i = 0; i < mi_size; i++)
	{
		mine[i] = rand() % (row*col - 1) + 1;
	}


	//�ߺ����� üŷ + �ٽ� �̱�
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

	//mine.stat�� ����
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

	//�޸� ��ȯ
	free(mine);
	return 0;
}

int mine_check(mine ** arg, int row, int col)
{
	//i = ��, j = ��
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