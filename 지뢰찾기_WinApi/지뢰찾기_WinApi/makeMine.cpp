#include "base.h"

#define mine_plus pMine[i][j].Periphery++;

int row, col;
int mineNum;
mine** pMine = NULL;

mine** makeMine(int diff)
{
	//난이도 따라서 칸 부여
	switch (diff)
	{
		case 1:
		{
			row = 9;
			col = 9;
			pMine = (mine**)calloc(row, sizeof(mine*));
			for (int i = 0; i < row; i++)
			{
				pMine[i] = (mine*)calloc(col, sizeof(mine));
			}
			break;
		}
		case 2:
		{
			row = 16;
			col = 16;
			pMine = (mine**)calloc(row, sizeof(mine*));
			for (int i = 0; i < row; i++)
			{
				pMine[i] = (mine*)calloc(col, sizeof(mine));
			}
			break;
		}
		case 3:
		{
			row = 16;
			col = 30;
			pMine = (mine**)calloc(row, sizeof(mine));
			for (int i = 0; i < row; i++)
			{
				pMine[i] = (mine*)calloc(col, sizeof(mine));
			}
			break;
		}
		default:
		{
			exit(0);
			break;
		}
	}

	//칸 번호 초기화
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			pMine[i][j].Number = i*j + 1;
		}
	}

	//pushMine 실행
	pushMine(diff);
	return pMine;
}

void pushMine(int diff)
{
	int* arg = NULL;

	//난이도 따라 지뢰 갯수 설정
	switch (diff)
	{
		case 1:
		{
			mineNum = 10;
			arg = (int*)calloc(mineNum, sizeof(int));
			break;
		}
		case 2:
		{
			mineNum = 40;
			arg = (int*)calloc(mineNum, sizeof(int));
			break;
		}
		case 3:
		{
			mineNum = 99;
			arg = (int*)calloc(mineNum, sizeof(int));
			break;
		}
		default:
		{
			exit(0);
			break;
		}
	}

	//난수로 지뢰 칸 선정 및 중복검사
	srand((unsigned)time(NULL));
	for (int i = 0; i < mineNum; i++)
	{
		arg[i] = rand() % (row*col) + 1;
	}
	int temp, count;
	mineLoop:
	for (int i = 0; i < mineNum; i++)
	{
		count = 0;
		temp = arg[i];
		for (int j = i; j < mineNum; j++)
		{
			if (j == mineNum - 1)
			{
				break;
			}
			else if (temp == arg[j])
			{
				arg[j] = rand() % (row*col) + 1;
			}
		}
		if (i == mineNum - 1 && count == 0)
		{
			break;
		}
		else if (count != 0)
		{
			goto mineLoop;
		}
	}

	//배열에 대입
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			for (int k = 0; k < mineNum; k++)
			{
				if (pMine[i][j].Number == arg[j])
				{
					pMine[i][j].Status = MINE;
				}
				else
				{
					pMine[i][j].Status = NOTMINE;
				}
			}
		}
	}

	free(arg);
	peripheral();
	return;
}

void peripheral()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			pMine[i][j].Periphery = 0;
			if (i == 0 && j == 0)
			{
				if (pMine[i][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j + 1].Status == MINE)
				{
					mine_plus
				}
			}
			else if (i == 0 && j == col - 1)
			{
				if (pMine[i][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j - 1].Status == MINE)
				{
					mine_plus
				}
			}
			else if (i == row - 1 && j == 0)
			{
				if (pMine[i][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j + 1].Status == MINE)
				{
					mine_plus
				}
			}
			else if (i == row - 1 && j == col - 1)
			{
				if (pMine[i - 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j - 1].Status == MINE)
				{
					mine_plus
				}
			}
			else if (i == 0)
			{
				if (pMine[i][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i][j + 1].Status == MINE)
				{
					mine_plus
				}
			}
			else if (i == row - 1)
			{
				if (pMine[i][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i][j + 1].Status == MINE)
				{
					mine_plus
				}
			}
			else if (j == 0)
			{
				if (pMine[i - 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j].Status == MINE)
				{
					mine_plus
				}
			}
			else if (j == col - 1)
			{
				if (pMine[i - 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j - 1].Status == MINE)
				{ 
					mine_plus
				}
				if (pMine[i][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j].Status == MINE)
				{
					mine_plus
				}
			}
			else
			{
				if (pMine[i - 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j - 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i + 1][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i][j + 1].Status == MINE)
				{
					mine_plus
				}
				if (pMine[i - 1][j + 1].Status == MINE)
				{
					mine_plus
				}
			}
		}
	}

	return;
}
