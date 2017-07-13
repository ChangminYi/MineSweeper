#include <stdio.h>
#include <stdlib.h>

//지뢰 구조체
typedef struct mine_status
{
	int num;
	int stat;
	int peri;
	int open;
}mine;

//전역변수 (행, 열)
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