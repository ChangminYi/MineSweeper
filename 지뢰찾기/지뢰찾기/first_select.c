//ù��° ���࿡�� ���� ������ �ʵ��� �����ϴ� �Լ�

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

int first_select(mine ** arg);
int first_select_vo(mine **arg, int openrow, int opencol);

int first_select(mine ** arg)
{
	//main �Լ����� extern���� �޾ƿ�
	extern row, col;
	//���ø�� ���ÿ� char
	char sel = 1;
	//���� ĭ �޾ƿ�
	int open_row = 0, open_col = 0;

	//���� ���� or ��ĭ ����
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

		first_select_vo(arg, open_row, open_col);

		return 1;
	}
	else if (sel == 'Y' || sel == 'y')	//����
	{
		printf("������ ĭ �Է��ϼ���(��): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("������ �� �Է��ϼ���(��): ");
		scanf_s("%d", &open_col);
		open_col--;

		select_mine(arg, open_row, open_col);

		return 1;
	}
}

int first_select_vo(mine **arg, int openrow, int opencol)
{
	//���� ĭ mine.open ����
	if (arg[openrow][opencol].stat == MINE)
	{
		do
		{
			mine_seed(arg, openrow, opencol);
			mine_check(arg, openrow, opencol);
		} while (arg[openrow][opencol].stat == MINE);
	}

	arg[openrow][opencol].open = TRUE;

	return 1;
}
