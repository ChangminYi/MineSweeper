#include <stdio.h>
#include <stdlib.h>

//���� ����ü
typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
}mine;

//�������� (��, ��)
int row = 0, col = 0;

void main()
{
	//�ʱ� ����
	mine** memloc = setting();

	mine_seed(memloc, row, col);
	mine_check(memloc, row, col);

	//���� ����
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