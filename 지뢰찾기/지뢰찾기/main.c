//main �Լ�

#include <stdio.h>

//���� ����ü
typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
	int sel;
}mine;

//�������� (��, ��)
int row = 0, col = 0;
int mi_size = 0;

void main()
{
	//�ʱ� ����
	mine** memloc = setting();
	int ret = 0;

	mine_seed(memloc, row, col);
	mine_check(memloc, row, col);

	//���� ����
	screen(memloc);
	first_select(memloc);
	do
	{
		screen(memloc);
		ret = select(memloc);
	} while (ret = 1);

	printf("\n\n");
	system("pause");

	return;
}