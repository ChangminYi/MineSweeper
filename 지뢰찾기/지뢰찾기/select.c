//���� üũ Ȥ�� ��ĭ üũ�ϴ� �Լ�

#include <stdio.h>

#define MINE 1
#define TRUE 1
#define FALSE 0
#define MINE_SEL 2
#define NOT_SEL 0

typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
	int sel;
}mine;

int select(mine ** arg);
int select_mine(mine **arg, int row, int col);
void select_inv(mine **arg);

int count = 0;

int select(mine ** arg)
{
	//���ø�� ���ÿ� char
	char sel = 1;
	//������ ĭ
	int open_row = 0, open_col = 0;

	//���� ���� or ��ĭ ����
	label:
	fflush(stdin);
	printf("���� ����(Y) / ��ĭ ����(N): ");
	scanf_s(" %c", &sel, 1);

	if (sel == 'N' || sel == 'n')	//��ĭ
	{
		printf("������ ĭ �Է��ϼ���(��): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("������ �� �Է��ϼ���(��): ");
		scanf_s("%d", &open_col);
		open_col--;

		int temp = winlose(arg, open_row, open_col);
		select_inv(arg);
		return temp;
	}
	else if (sel == 'Y' || sel == 'y')	//����
	{
		printf("������ ĭ �Է��ϼ���(��): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("������ �� �Է��ϼ���(��): ");
		scanf_s("%d", &open_col);
		open_col--;

		int temp = select_mine(arg, open_row, open_col);
		return temp;
	}
	else
	{
		screen(arg);
		goto label;
	}
}

int select_mine(mine **arg, int row, int col)
{
	if (arg[row][col].open == TRUE)
	{
		printf("�̹� ���� ĭ�Դϴ�.\n");
		system("pause");
	}
	else if (arg[row][col].open == FALSE && arg[row][col].sel == FALSE)
	{
		arg[row][col].sel = MINE_SEL;
		if (arg[row][col].stat == MINE)
		{
			count++;
		}
	}
	else if (arg[row][col].open == FALSE && arg[row][col].sel == MINE_SEL)
	{
		arg[row][col].sel = FALSE;
		if (arg[row][col].stat == MINE);
		{
			count--;
		}
	}

	return 1;
}

void select_inv(mine ** arg)
{
	//�� ũ�� �޾ƿ�
	extern row, col;

	for (int k = 0; k < row*col; k++)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (i == 0 && j == 0)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i][j + 1].open == FALSE)
						{
							arg[i][j + 1].open = TRUE;
						}
						if (arg[i + 1][j + 1].open == FALSE)
						{
							arg[i + 1][j + 1].open = TRUE;
						}
						if (arg[i + 1][j].open == FALSE)
						{
							arg[i + 1][j].open = TRUE;
						}
					}
				}
				else if (i == 0 && j == col - 1)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i + 1][j].open == FALSE)
						{
							arg[i + 1][j].open = TRUE;
						}
						if (arg[i + 1][j - 1].open == FALSE)
						{
							arg[i + 1][j - 1].open = TRUE;
						}
						if (arg[i][j - 1].open == FALSE)
						{
							arg[i][j - 1].open = TRUE;
						}
					}
				}
				else if (i == row - 1 && j == 0)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i - 1][j].open == FALSE)
						{
							arg[i - 1][j].open = TRUE;
						}
						if (arg[i - 1][j + 1].open == FALSE)
						{
							arg[i - 1][j + 1].open = TRUE;
						}
						if (arg[i][j + 1].open == FALSE)
						{
							arg[i][j + 1].open = TRUE;
						}
					}
				}
				else if (i == row - 1 && j == col - 1)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i][j - 1].open == FALSE)
						{
							arg[i][j - 1].open = TRUE;
						}
						if (arg[i - 1][j - 1].open == FALSE)
						{
							arg[i - 1][j - 1].open = TRUE;
						}
						if (arg[i - 1][j].open == FALSE)
						{
							arg[i - 1][j].open = TRUE;
						}
					}
				}
				else if (i == 0)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i][j + 1].open == FALSE)
						{
							arg[i][j + 1].open = TRUE;
						}
						if (arg[i + 1][j + 1].open == FALSE)
						{
							arg[i + 1][j + 1].open = TRUE;
						}
						if (arg[i + 1][j].open == FALSE)
						{
							arg[i + 1][j].open = TRUE;
						}
						if (arg[i + 1][j - 1].open == FALSE)
						{
							arg[i + 1][j - 1].open = TRUE;
						}
						if (arg[i][j - 1].open == FALSE)
						{
							arg[i][j - 1].open = TRUE;
						}
					}
				}
				else if (i == row - 1)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i][j - 1].open == FALSE)
						{
							arg[i][j - 1].open = TRUE;
						}
						if (arg[i - 1][j - 1].open == FALSE)
						{
							arg[i - 1][j - 1].open = TRUE;
						}
						if (arg[i - 1][j].open == FALSE)
						{
							arg[i - 1][j].open = TRUE;
						}
						if (arg[i - 1][j + i].open == FALSE)
						{
							arg[i - 1][j + 1].open = TRUE;
						}
						if (arg[i][j + 1].open == FALSE)
						{
							arg[i][j + 1].open = TRUE;
						}
					}
				}
				else if (j == 0)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i - 1][j].open == FALSE)
						{
							arg[i - 1][j].open = TRUE;
						}
						if (arg[i - 1][j + 1].open == FALSE)
						{
							arg[i - 1][j + 1].open = TRUE;
						}
						if (arg[i][j + 1].open == FALSE)
						{
							arg[i][j + 1].open = TRUE;
						}
						if (arg[i + 1][j + 1].open == FALSE)
						{
							arg[i + 1][j + 1].open = TRUE;
						}
						if (arg[i + 1][j].open == FALSE)
						{
							arg[i + 1][j].open = TRUE;
						}
					}
				}
				else if (j == col - 1)
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i - 1][j].open == FALSE)
						{
							arg[i - 1][j].open = TRUE;
						}
						if (arg[i - 1][j - 1].open == FALSE)
						{
							arg[i - 1][j - 1].open = TRUE;
						}
						if (arg[i][j - 1].open == FALSE)
						{
							arg[i][j - 1].open = TRUE;
						}
						if (arg[i + 1][j - 1].open == FALSE)
						{
							arg[i + 1][j - 1].open = TRUE;
						}
						if (arg[i + 1][j].open == FALSE)
						{
							arg[i + 1][j].open = TRUE;
						}
					}
				}
				else
				{
					if (arg[i][j].stat == MINE)
					{
						continue;
					}
					else if (arg[i][j].open == TRUE && arg[i][j].peri == 0)
					{
						if (arg[i - 1][j].open == FALSE)
						{
							arg[i - 1][j].open = TRUE;
						}
						if (arg[i - 1][j + 1].open == FALSE)
						{
							arg[i - 1][j + 1].open = TRUE;
						}
						if (arg[i][j + 1].open == FALSE)
						{
							arg[i][j + 1].open = TRUE;
						}
						if (arg[i + 1][j + 1].open == FALSE)
						{
							arg[i + 1][j + 1].open = TRUE;
						}
						if (arg[i + 1][j].open == FALSE)
						{
							arg[i + 1][j].open = TRUE;
						}
						if (arg[i + 1][j - 1].open == FALSE)
						{
							arg[i + 1][j - 1].open = TRUE;
						}
						if (arg[i][j - 1].open == FALSE)
						{
							arg[i][j - 1].open = TRUE;
						}
						if (arg[i - 1][j - 1].open == FALSE)
						{
							arg[i - 1][j - 1].open = TRUE;
						}
					}
				}
			}
		}
	}
}
