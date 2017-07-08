#include <stdio.h>
#include <stdlib.h>

int* setting();

void main()
{
	setting();

	printf("\n\n");
	system("pause");
}

int* setting()
{
	int row = 0, col = 0;

	//size confirming...
	printf("난이도 선택\n");
	printf("가로줄 갯수: ");
	scanf_s("%d", &col);
	printf("세로줄 갯수: ");
	scanf_s("%d", &row);

	//memory allocating & initializing...
	int **po_col = (int **)calloc(row, sizeof(int));
	for (int i = 0; i < row; i++)
	{
		po_col[i] = (int *)calloc(col, sizeof(int));
		for (int r = 0; r < col; r++)
		{
			po_col[i][r] = i*col + r + 1;
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
			printf("%5d", po_col[n][m]);
		}
		printf("\n");
	}

	// mem-free... (not needed)
	/*
	for (int k = 0; k < row; k++)
	{
	free(po_col[k]);
	}
	free(po_col);
	*/

	return &po_col;
}