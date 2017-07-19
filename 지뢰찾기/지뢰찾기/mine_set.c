//지뢰찾기 판에 지뢰 설정하는 함수

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
	extern row, col, mi_size;
	int level = 0;

	//지뢰찾기 판 크기 설정
ret:
	system("cls");
	printf("난이도 선택\n");
	printf("1. 초급(9x9, 지뢰 10개)\n");
	printf("2. 중급(16x16, 지뢰 40개)\n");
	printf("3. 상급(30x16, 지뢰 85개)\n");
	printf("4. 사용자 지정\n>> ");
	scanf_s("%d", &level);

	switch (level)
	{
	case 1:
		row = 9;
		col = 9;
		mi_size = 10;
		break;
	case 2:
		row = 16;
		col = 16;
		mi_size = 40;
		break;
	case 3:
		row = 16;
		col = 30;
		mi_size = 85;
		break;
	case 4:
		printf("가로줄 개수: ");
		scanf_s("%d", &row);
		printf("세로줄 개수: ");
		scanf_s("%d", &col);
		printf("지뢰 개수: ");
		scanf_s("%d", &mi_size);
		break;
	default:
		goto ret;
		break;
	}
	
	//메모리 할당 + mine.num 초기화
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

	//주소값 반환
	return col_p;
}

int mine_seed(mine ** argv, int row, int col)
{
	//지뢰 갯수
	extern mi_size;

	//지뢰 갯수 설정 위한 배열
	int *mine = (int *)calloc(mi_size, sizeof(int));

	//지뢰 설정될 칸 난수함수로 설정
	srand((unsigned)time(NULL));
	for (int i = 0; i < mi_size; i++)
	{
		mine[i] = rand() % (row*col - 1) + 1;
	}

	//중복숫자 체킹 + 다시 뽑기
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

	//mine.stat에 적용
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
			}
		}
	}

	//메모리 반환
	free(mine);
	return 0;
}

int mine_check(mine ** arg, int row, int col)
{
	//i = 행, j = 열
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