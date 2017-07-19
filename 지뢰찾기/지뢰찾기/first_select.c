//첫번째 실행에서 지뢰 나오지 않도록 설정하는 함수

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
	//main 함수에서 extern으로 받아옴
	extern row, col;
	//선택모드 선택용 char
	char sel = 1;
	//열린 칸 받아옴
	int open_row = 0, open_col = 0;

	//지뢰 선택 or 빈칸 선택
	printf("지뢰 선택(Y) / 빈칸 선택(N): ");
	scanf_s(" %c", &sel, 1);

	if (sel == 'N' || sel == 'n')	//빈칸
	{
		printf("선택할 칸 입력하세요(행): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("선택할 행 입력하세요(열): ");
		scanf_s("%d", &open_col);
		open_col--;

		first_select_vo(arg, open_row, open_col);

		return 1;
	}
	else if (sel == 'Y' || sel == 'y')	//지뢰
	{
		printf("선택할 칸 입력하세요(행): ");
		scanf_s("%d", &open_row);
		open_row--;
		printf("선택할 행 입력하세요(열): ");
		scanf_s("%d", &open_col);
		open_col--;

		select_mine(arg, open_row, open_col);

		return 1;
	}
}

int first_select_vo(mine **arg, int openrow, int opencol)
{
	//열린 칸 mine.open 수정
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
