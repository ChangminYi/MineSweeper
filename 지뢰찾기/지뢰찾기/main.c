#include <stdio.h>
#include <stdlib.h>

typedef struct mine_status
{
	int num;
	int peri;
}mine;

mine* setting();
int check(mine* loc, int row, int col);

int row = 0, col = 0;

void main()
{
	int mem = setting();
	do
	{
		check(mem, row, col);
	} while (check(mem, row, col) != 0);

	printf("\n\n");
	system("pause");
}

mine* setting()
{
	//size confirming...
	printf("난이도 선택\n");
	printf("가로줄 갯수: ");
	scanf_s("%d", &col);
	printf("세로줄 갯수: ");
	scanf_s("%d", &row);

	//memory allocating & initializing...
	mine **po_col = (mine **)calloc(row, sizeof(mine));
	for (int i = 0; i < row; i++)
	{
		po_col[i] = (mine *)calloc(col, sizeof(mine));
		for (int r = 0; r < col; r++)
		{
			po_col[i][r].num = i*col + r + 1;
		}
	}

	// mem-alloc confirmation...
	printf("주소: %d\n", &po_col);
	printf("각 열 주소:\n");
	for (int j = 0; j < row; j++)
	{
		printf("\t%d\n", po_col[j]);
	}
	printf("\n\n");
	for (int n = 0; n < row; n++)
	{
		for (int m = 0; m < col; m++)
		{
			printf("%5d", po_col[n][m].num);
		}
		printf("\n");
	}

	// mem-free... (not needed)
	for (int k = 0; k < row; k++)
	{
	free(po_col[k]);
	}
	free(po_col);

	return &po_col;
}

int check(mine* loc, int row, int col)
{
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{

		}
	}
	return 0;
}
