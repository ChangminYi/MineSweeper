//���� üũ Ȥ�� ��ĭ üũ�ϴ� �Լ�

#include <stdio.h>
#include <stdlib.h>

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
}mine;

int select(mine ** arg);
int select_mine(mine **arg, int row, int col);

int select(mine ** arg)
{
	//���ø�� ���ÿ� char
	char sel = 1;
	//������ ĭ
	int open_row = 0, open_col = 0;

	//���� ���� or ��ĭ ����
	printf("���� ����(Y) / ��ĭ ����(N)");
	scanf_s("%c", &sel, 1);
	if (sel == 'N' || sel == "n")	//��ĭ
	{
		printf("������ ĭ �Է��ϼ���(��): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("������ �� �Է��ϼ���(��): ");
		scanf_s("%d", &open_col);
		open_col--;

		return winlose(arg, open_row, open_col);
	}
	else if (sel == 'Y' || sel == 'y')	//����
	{
		printf("������ ĭ �Է��ϼ���(��): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("������ �� �Է��ϼ���(��): ");
		scanf_s("%d", &open_col);
		open_col--;

		return select_mine(arg, open_row, open_col);
	}
}

int select_mine(mine **arg, int row, int col)
{
	if (arg[row][col].stat == MINE && arg[row][col].peri != 0)
	{
		arg[row][col].stat = MINE_SEL;
	}
	else if (arg[row][col].stat == MINE_SEL && arg[row][col].peri != 0)
	{
		arg[row][col].stat == MINE;
	}
	else if (arg[row][col].open== TRUE && arg[row][col].peri == 0)
	{
		printf("�̹� ���� ĭ�Դϴ�.");
	}

	return 1;
}