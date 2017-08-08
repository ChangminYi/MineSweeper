////////////////////////////////////////////////
////////////////////////////////////////////////
//��� ����
#include <Windows.h>
#include <time.h>
#include "resource.h"	//��Ʈ�� ����� ���� ���ҽ� ������� (resource.rc ����)
#include "D:\Visual Studio 2017\����ã��_SDK�����ذ�\Win32Project3\define_macro.h"	//��ũ�� ������� (�� ������ �����η� ã�� -> ���� �� ����η� �ٲ㼭...)

////////////////////////////////////////////////
////////////////////////////////////////////////
/*�Լ��� (�ڼ��� ������ �Լ� ���� ���� �����)*/
//
void setBackgroundSize(LPRECT clientRect);
void setGameState(int gameMode, LPRECT clientRect);
void generateMine(int tileIndex);
void checkTileState(int indexRow, int indexCol);
void checkOpenTiles(int indexRow, int indexCol);
void checkMines();
void clearTiles();
int getAdjustMineCount(int indexRow, int indexCol);
int getAdjustFlagCount(int indexRow, int indexCol);
void drawBitmap(HDC hdc, int x, int y, HBITMAP hBitmap);
void drawNumber(HDC hdc, int x, int y, int drawNumber);
void printMineSweeper();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

////////////////////////////////////////////////
////////////////////////////////////////////////
//������
int remainMine;   //�����ִ� ���� ��.
int stageMine;	   //���̵� ���� ������ ���� ��
int closedTile;   //���� �ִ� Ÿ�� ��.  
int gameMode;	//���� ���̵�
int gameState;	//���� �����Ȳ
int currentTileWidth;	//�� �ʺ�
int currentTileHeight;	//�� ����
double playTime;	//��� �ð�
int background_L;	//�� �º� ��ǥ
int background_R;	//�� �캯 ��ǥ
int background_T;	//�� ���� ��ǥ
int background_B;	//�� �Ʒ��� ��ǥ
bool L_Pressed;	//���콺 ��Ŭ�� Ȯ�ο� ����
bool R_Pressed; //���콺 ��Ŭ��		"

//�� ���� �迭 (����ü�� �� ���� ����)
typedef struct MINE_STATUS
{
int tile_MineCount;
int tile_MineState;
bool tile_mineExist;
}mine;
mine Mine[HARD_TILE_WIDTH * HARD_TILE_HEIGHT];

//�ν��Ͻ� �ڵ�, ������ �ڵ�, ��Ʈ��
HINSTANCE g_hInst;
HWND hWnd, hStartButton;
HBITMAP memoryBitmap;	// ���� �޸� ��Ʈ��. ȭ���� �޸𸮿� �׸��� �� �� WndProc: WM_PAINT���� �޸� ��Ʈ���� �׸�.


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//������
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd)
{
	MSG msg;
	LPSTR lpszClass = "MYButton";
	WNDCLASS wc;
	g_hInst = hInstance;

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"WINDOW";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&wc);

	hWnd = CreateWindow(L"WINDOW", L"WINDOW", WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, ShowCmd);

	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//WndProc �Լ� (WinMain���� ���� �޽��� �ؼ�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memDC;
	PAINTSTRUCT ps;
	RECT tempRect;
	HBITMAP hOldBmp;
	int mouseX, mouseY, index, indexRow, indexColumm, leftX, TopY;
	int nMatchMineCount, buttonWidth, buttonHeight;

	//�޽��� ó���� switch��
	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &tempRect);
		setGameState(HARD_MODE, &tempRect);

		buttonWidth = 100;
		buttonHeight = 30;
		leftX = tempRect.right / 2; //���� ���� ����
		TopY = 20;	//���� ���� ����
		leftX -= (buttonWidth / 2);

		//���� �õ�
		srand((int)time(NULL));

		hStartButton = CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftX, TopY, buttonWidth, buttonHeight, hWnd, (HMENU)ID_START, g_hInst, NULL);
		L_Pressed = false;
		R_Pressed = false;

		//Ÿ�̸�
		SetTimer(hWnd, 1, 500, NULL);

		//ȭ�� ���
		printMineSweeper();

		break;

	case WM_TIMER:
		if (gameState == GAME_PLAY)
		{
			playTime += 0.5f;
			if (playTime > 999.0f)
			{
				playTime = 999.0f;
			}

			printMineSweeper();
			InvalidateRect(hWnd, NULL, FALSE);	//ȭ�� �ٽ� ��½�Ű�� �Լ�
		}

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineSweeper();
			InvalidateRect(hWnd, NULL, FALSE);

			break;

		case ID_GAME_NEWGAME:
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineSweeper();
			InvalidateRect(hWnd, NULL, FALSE);

			break;

		case ID_MODE_EASY:
			gameMode = EASY_MODE;
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineSweeper();
			InvalidateRect(hWnd, NULL, TRUE);

			break;

		case ID_MODE_MIDDLE:
			gameMode = NORMAL_MODE;
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineSweeper();
			InvalidateRect(hWnd, NULL, TRUE);

			break;

		case ID_MODE_HARD:
			gameMode = HARD_MODE;
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineSweeper();
			InvalidateRect(hWnd, NULL, TRUE);

			break;

		case ID_GAME_EXIT:
			PostQuitMessage(0);

			break;

		}

		break;

	case WM_SIZE:
		GetClientRect(hWnd, &tempRect);
		setBackgroundSize(&tempRect);

		buttonWidth = 100;
		buttonHeight = 30;
		leftX = tempRect.right / 2;
		TopY = 20;
		leftX -= (buttonWidth / 2);

		MoveWindow(hStartButton, leftX, TopY, buttonWidth, buttonHeight, TRUE);

		printMineSweeper();
		InvalidateRect(hWnd, NULL, TRUE);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hdc);
		hOldBmp = (HBITMAP)SelectObject(memDC, memoryBitmap);
		BitBlt(hdc, 0, 0, background_R, background_B, memDC, 0, 0, SRCCOPY);
		SelectObject(memDC, hOldBmp);
		DeleteDC(memDC);
		EndPaint(hWnd, &ps);

		return 0;

	case WM_LBUTTONDOWN:
		L_Pressed = true;
		if (R_Pressed)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			if (mouseX > background_L && mouseX < background_R && mouseY > background_T && mouseY < background_B)
			{
				indexColumm = (mouseX - background_L) / TILE_IMAGE_WIDTH;
				indexRow = (mouseY - background_T) / TILE_IMAGE_HEIGHT;	//index: ���� ĭ�� ��ȣ
				index = indexRow * currentTileWidth + indexColumm;
				if (gameState == GAME_PLAY)
				{
					if (Mine[index].tile_MineState == TILE_STATE_OPEN)
					{
						checkOpenTiles(indexRow, indexColumm);
					}
				}

				printMineSweeper();
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		break;

	case WM_RBUTTONDOWN:
		R_Pressed = true;
		if (L_Pressed)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			if (mouseX > background_L && mouseX < background_R && mouseY > background_T && mouseY < background_B)
			{
				indexColumm = (mouseX - background_L) / TILE_IMAGE_WIDTH;
				indexRow = (mouseY - background_T) / TILE_IMAGE_HEIGHT;
				index = indexRow * currentTileWidth + indexColumm;
				if (gameState == GAME_PLAY)
				{
					if (Mine[index].tile_MineState == TILE_STATE_OPEN)
					{
						checkOpenTiles(indexRow, indexColumm);
					}
				}

				printMineSweeper();
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}

		break;

	case WM_LBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		if (mouseX > background_L && mouseX < background_R && mouseY > background_T && mouseY < background_B)
		{
			indexColumm = (mouseX - background_L) / TILE_IMAGE_WIDTH;
			indexRow = (mouseY - background_T) / TILE_IMAGE_HEIGHT;
			index = indexRow * currentTileWidth + indexColumm;
			switch (gameState)
			{
			case GAME_READY:
				generateMine(index);
				checkTileState(indexRow, indexColumm);

				gameState = GAME_PLAY;	//Ÿ�̸� �ʱ�ȭ, ��� Play�� ����

				break;

			case GAME_PLAY:
				if (Mine[index].tile_MineState == TILE_STATE_CLOSE)
				{
					if (Mine[index].tile_mineExist == true)
					{
						Mine[index].tile_MineState = TILE_STATE_BOMB_MINE;
						gameState = GAME_OVER;
						MessageBox(hWnd, L"Game Over", L"Game Over", MB_OK);

						clearTiles();
					}
					else
					{
						checkTileState(indexRow, indexColumm);
					}
				}
				if (closedTile == remainMine)
					checkMines();

				break;

			}
		}
		L_Pressed = false;
		printMineSweeper();
		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case WM_RBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		nMatchMineCount = 0;
		if (mouseX > background_L && mouseX < background_R && mouseY > background_T && mouseY < background_B)
		{
			indexColumm = (mouseX - background_L) / TILE_IMAGE_WIDTH;
			indexRow = (mouseY - background_T) / TILE_IMAGE_HEIGHT;
			index = indexRow * currentTileWidth + indexColumm;
			if (gameState == GAME_PLAY)
			{
				if (Mine[index].tile_MineState == TILE_STATE_CLOSE)
				{
					Mine[index].tile_MineState = TILE_STATE_FLAG;
					closedTile--;
					remainMine--;
				}
				else if (Mine[index].tile_MineState == TILE_STATE_FLAG)
				{
					remainMine++;
					closedTile++;

					Mine[index].tile_MineState = TILE_STATE_QUESTION;
				}
				else if (Mine[index].tile_MineState == TILE_STATE_QUESTION)
				{
					Mine[index].tile_MineState = TILE_STATE_CLOSE;
				}
			}
		}
		L_Pressed = false;
		printMineSweeper();
		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//���⼭���� �Լ��� ����

//ó�� ���� ���� �ʱ�ȭ�ϴ� �Լ�
void setGameState(int gameMode, LPRECT clientRect)
{
	int nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;
	gameMode = gameMode;
	gameState = GAME_READY;
	playTime = 0.0f;
	setBackgroundSize(clientRect);
	for (int i = 0; i < nAllTileCount; i++)
	{
		Mine[i].tile_MineCount = 0;
		Mine[i].tile_MineState = TILE_STATE_CLOSE;
		Mine[i].tile_mineExist = false;
	}

	return;
}

//��Ʈ�� ����ϴ� �Լ�
void drawBitmap(HDC hdc, int x, int y, HBITMAP hBitmap)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int imageHeight, imageWidth;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);
	GetObject(hBitmap, sizeof(BITMAP), &bit);
	imageWidth = bit.bmWidth;
	imageHeight = bit.bmHeight;
	BitBlt(hdc, x, y, imageWidth, imageHeight, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(OldBitmap);
	DeleteDC(MemDC);

	return;
}

//���� �����ϴ� �Լ�
void generateMine(int tileIndex)
{
	int nGenratedMine = 0;
	int tempIndex, tempMineCount, nAllTileCount;
	int indexRow, indexCol;

	switch (gameMode)
	{
	case EASY_MODE:
		nAllTileCount = EASY_TILE_WIDTH * EASY_TILE_HEIGHT;
		break;

	case NORMAL_MODE:
		nAllTileCount = NORMAL_TILE_WIDTH * NORMAL_TILE_HEIGHT;
		break;

	case HARD_MODE:
		nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;
		break;
	}
	closedTile = nAllTileCount;

	while (nGenratedMine != remainMine)
	{
		tempIndex = rand() % nAllTileCount;
		if (tempIndex != tileIndex && !Mine[tempIndex].tile_mineExist)
		{
			Mine[tempIndex].tile_mineExist = true;
			nGenratedMine++;
		}
	}

	for (int i = 0; i < nAllTileCount; i++)
	{
		indexRow = i / currentTileWidth;
		indexCol = i % currentTileWidth;
		tempMineCount = getAdjustMineCount(indexRow, indexCol);
		Mine[i].tile_MineCount = tempMineCount;
	}

	return;
}

//Ÿ���� ������ �� ����Ǵ� üũ�Լ�
void checkTileState(int indexRow, int indexCol)
{
	int nAllTileCount, iPrevTileState;
	int  index;

	switch (gameMode)
	{
	case EASY_MODE:
		nAllTileCount = EASY_TILE_WIDTH * EASY_TILE_HEIGHT;
		break;

	case NORMAL_MODE:
		nAllTileCount = NORMAL_TILE_WIDTH * NORMAL_TILE_HEIGHT;
		break;

	case HARD_MODE:
		nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;
		break;
	}

	index = (indexRow * currentTileWidth) + indexCol;
	iPrevTileState = Mine[index].tile_MineState;

	if (!(iPrevTileState == TILE_STATE_CLOSE || iPrevTileState == TILE_STATE_QUESTION))
	{
		return;
	}
	if (indexRow < 0 || indexRow >= currentTileHeight || indexCol < 0 || indexCol >= currentTileWidth)
	{
		return;
	}

	Mine[index].tile_MineState = TILE_STATE_OPEN;
	closedTile--;

	if (Mine[index].tile_mineExist)
	{
		gameState = GAME_OVER;

		if (iPrevTileState == TILE_STATE_QUESTION)
		{
			Mine[index].tile_MineState = TILE_STATE_BOMB_QUESTION;
		}
		else
		{
			Mine[index].tile_MineState = TILE_STATE_BOMB_MINE;
		}

		clearTiles();
		MessageBox(hWnd, L"Mission Failed", L"Game Over", MB_OK);

		return;
	}

	if (getAdjustMineCount(indexRow, indexCol) > 0)
	{
		return;
	}

	checkTileState(indexRow - 1, indexCol - 1);
	checkTileState(indexRow - 1, indexCol);
	checkTileState(indexRow - 1, indexCol + 1);
	checkTileState(indexRow, indexCol - 1);
	checkTileState(indexRow, indexCol + 1);
	checkTileState(indexRow + 1, indexCol - 1);
	checkTileState(indexRow + 1, indexCol);
	checkTileState(indexRow + 1, indexCol + 1);

	return;
}

//���� Ÿ�� üũ�ϴ� �Լ�
void checkOpenTiles(int indexRow, int indexCol)
{
	int nAllTileCount;
	int  index, iCurrTileState;

	switch (gameMode)
	{
	case EASY_MODE:
		nAllTileCount = EASY_TILE_WIDTH * EASY_TILE_HEIGHT;
		break;

	case NORMAL_MODE:
		nAllTileCount = NORMAL_TILE_WIDTH * NORMAL_TILE_HEIGHT;
		break;

	case HARD_MODE:
		nAllTileCount = HARD_TILE_WIDTH * HARD_TILE_HEIGHT;
		break;
	}
	index = (indexRow * currentTileWidth) + indexCol;

	if (Mine[index].tile_MineState != TILE_STATE_OPEN)
	{
		return;
	}
	if (indexRow < 0 || indexRow >= currentTileHeight || indexCol < 0 || indexCol >= currentTileWidth)
	{
		return;
	}
	if (getAdjustFlagCount(indexRow, indexCol) != Mine[index].tile_MineCount)
	{
		return;
	}

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)	//�»���
	{
		iCurrTileState = Mine[index - currentTileWidth - 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol - 1);
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)	//�߻���
	{
		iCurrTileState = Mine[index - currentTileWidth].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol);
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)	//�����
	{
		iCurrTileState = Mine[index - currentTileWidth + 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol + 1);
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)	//������
	{
		iCurrTileState = Mine[index - 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow, indexCol - 1);
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)	//������
	{
		iCurrTileState = Mine[index + 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow, indexCol + 1);
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)	//������
	{
		iCurrTileState = Mine[index + currentTileWidth - 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol - 1);
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)	//������
	{
		iCurrTileState = Mine[index + currentTileWidth].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol);
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)	//������
	{
		iCurrTileState = Mine[index + currentTileWidth + 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol + 1);
		}
	}

	return;
}


//���� �� ���� �Լ�
int getAdjustMineCount(int indexRow, int indexCol)
{
	int MineCount = 0;
	int index = (indexRow * currentTileWidth) + indexCol;

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)	//�»���
	{
		if (Mine[index - currentTileWidth - 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)	//�߻���
	{
		if (Mine[index - currentTileWidth].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)	//�����
	{
		if (Mine[index - currentTileWidth + 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)	//������
	{
		if (Mine[index - 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)	//������
	{
		if (Mine[index + 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)	//������
	{
		if (Mine[index + currentTileWidth - 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)	//������
	{
		if (Mine[index + currentTileWidth].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)	//������
	{
		if (Mine[index + currentTileWidth + 1].tile_mineExist)
		{
			MineCount++;
		}
	}

	return MineCount;
}

//��� ���� �Լ�
int getAdjustFlagCount(int indexRow, int indexCol)
{
	int FlagCount = 0;
	int index = (indexRow * currentTileWidth) + indexCol;

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)	//�»���
	{
		if (Mine[index-currentTileWidth-1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)	//�߻���
	{
		if (Mine[index-currentTileWidth].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)	//�����
	{
		if (Mine[index-currentTileWidth+1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)	//������
	{
		if (Mine[index-1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)	//������
	{
		if (Mine[index+1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)	//������
	{
		if (Mine[index+currentTileWidth-1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)	//������
	{
		if (Mine[index+currentTileWidth].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)	//������
	{
		if (Mine[index+currentTileWidth+1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}

	return FlagCount;
}

//����(�ð�) ����ϴ� �Լ�
void drawNumber(HDC hdc, int x, int y, int drawNumber)
{
	HBITMAP numBitmap;

	switch (drawNumber)
	{
	case 0:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER0));
		break;

	case 1:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER1));
		break;

	case 2:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER2));
		break;

	case 3:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER3));
		break;

	case 4:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER4));
		break;

	case 5:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER5));
		break;

	case 6:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER6));
		break;

	case 7:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER7));
		break;

	case 8:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER8));
		break;

	case 9:
		numBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NUMBER9));
		break;
	}

	drawBitmap(hdc, x, y, numBitmap);
	DeleteObject(numBitmap);

	return;
}

//�� ������ �����ϴ� �Լ�
void setBackgroundSize(LPRECT clientRect)
{
	int nImageWidthGap, nImageHeightGap;

	switch (gameMode)
	{
	case EASY_MODE:
		nImageWidthGap = ((clientRect->right - clientRect->left) - (TILE_IMAGE_WIDTH * EASY_TILE_WIDTH)) / 2;
		nImageHeightGap = ((clientRect->bottom - clientRect->top) - (TILE_IMAGE_HEIGHT * EASY_TILE_HEIGHT)) / 2;
		remainMine = EASY_MINE_COUNT;
		currentTileWidth = EASY_TILE_WIDTH;
		currentTileHeight = EASY_TILE_HEIGHT;

		break;

	case NORMAL_MODE:
		nImageWidthGap = ((clientRect->right - clientRect->left) - (TILE_IMAGE_WIDTH * NORMAL_TILE_WIDTH)) / 2;
		nImageHeightGap = ((clientRect->bottom - clientRect->top) - (TILE_IMAGE_HEIGHT * NORMAL_TILE_HEIGHT)) / 2;
		remainMine = NORMAL_MINE_COUNT;
		currentTileWidth = NORMAL_TILE_WIDTH;
		currentTileHeight = NORMAL_TILE_HEIGHT;

		break;

	case HARD_MODE:
		nImageWidthGap = ((clientRect->right - clientRect->left) - (TILE_IMAGE_WIDTH * HARD_TILE_WIDTH)) / 2;
		nImageHeightGap = ((clientRect->bottom - clientRect->top) - (TILE_IMAGE_HEIGHT * HARD_TILE_HEIGHT)) / 2;
		remainMine = HARD_MINE_COUNT;
		currentTileWidth = HARD_TILE_WIDTH;
		currentTileHeight = HARD_TILE_HEIGHT;

		break;
	}

	stageMine = remainMine;
	background_L = nImageWidthGap + clientRect->left;
	background_T = nImageHeightGap + clientRect->top;
	background_R = clientRect->right - nImageWidthGap;
	background_B = clientRect->bottom - nImageHeightGap;

	return;
}

//��ü ȭ�� ����ϴ� �Լ�
void printMineSweeper()
{
	HDC hdc, memDC;
	HBITMAP tileFlag, tileClose, tileMine0, tileMine1, tileMine2, tileMine3, tileMine4, tileMine5, tileMine6, tileMine7, tileMine8, tileQuestion, tileMine, failFlag, bombMine, bombQuestion, hOldBmp, numMinus;
	int index, indexRow, indexColumm, leftX, TopY, nAllTileCount;

	if (memoryBitmap)	//�̹� ��µ� ȭ�� ����
	{
		DeleteObject(memoryBitmap);
	}

	hdc = GetDC(hWnd);
	memoryBitmap = CreateCompatibleBitmap(hdc, background_R, background_B);
	memDC = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(memDC, memoryBitmap);
	PatBlt(memDC, 0, 0, background_R, background_B, PATCOPY);

	tileFlag = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLAG));
	tileClose = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLOSE));
	tileMine0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE0));
	tileMine1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE1));
	tileMine2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE2));
	tileMine3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE3));
	tileMine4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE4));
	tileMine5 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE5));
	tileMine6 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE6));
	tileMine7 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE7));
	tileMine8 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE8));
	tileQuestion = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_QUESTION));
	tileMine = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE));
	failFlag = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FAIL_FLAG));
	bombMine = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOMB_MINE));
	bombQuestion = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOMB_QUESTION));
	numMinus = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINUS));

	switch (gameMode)
	{
	case EASY_MODE:
		nAllTileCount = EASY_TILE_HEIGHT * EASY_TILE_WIDTH;
		break;

	case NORMAL_MODE:
		nAllTileCount = NORMAL_TILE_HEIGHT * NORMAL_TILE_WIDTH;
		break;

	case HARD_MODE:
		nAllTileCount = HARD_TILE_HEIGHT * HARD_TILE_WIDTH;
		break;
	}

	index = ((int)playTime) % 10;
	leftX = background_L + (NUMBER_IMAGE_WIDTH * 3);
	TopY = background_T - (NUMBER_IMAGE_HEIGHT * 2);
	drawNumber(memDC, leftX, TopY, index);
	index = ((int)playTime) / 10;

	if (index == 26)
	{
		index = index;
	}
	if (index >= 10)
	{
		index %= 10;
	}

	leftX -= NUMBER_IMAGE_WIDTH;
	drawNumber(memDC, leftX, TopY, index);

	index = ((int)playTime) / 100;
	leftX -= NUMBER_IMAGE_WIDTH;
	drawNumber(memDC, leftX, TopY, index);

	index = abs(remainMine) % 10;
	leftX = background_R - (NUMBER_IMAGE_WIDTH * 2);
	TopY = background_T - (NUMBER_IMAGE_HEIGHT * 2);
	drawNumber(memDC, leftX, TopY, index);

	index = abs(remainMine) / 10;
	leftX -= NUMBER_IMAGE_WIDTH;
	drawNumber(memDC, leftX, TopY, index);

	if (remainMine < 0)
	{
		leftX -= NUMBER_IMAGE_WIDTH;
		drawBitmap(memDC, leftX, TopY, numMinus);
	}

	for (int i = 0; i < nAllTileCount; i++)
	{
		indexRow = i / currentTileWidth;
		indexColumm = i % currentTileWidth;
		leftX = background_L + (indexColumm * TILE_IMAGE_WIDTH);
		TopY = background_T + (indexRow * TILE_IMAGE_HEIGHT);

		switch (Mine[i].tile_MineState)
		{
		case TILE_STATE_CLOSE:
			drawBitmap(memDC, leftX, TopY, tileClose);
			break;

		case TILE_STATE_QUESTION:
			drawBitmap(memDC, leftX, TopY, tileQuestion);
			break;

		case TILE_STATE_FLAG:
			drawBitmap(memDC, leftX, TopY, tileFlag);
			break;

		case TILE_STATE_BOMB_MINE:
			drawBitmap(memDC, leftX, TopY, bombMine);
			break;

		case TILE_STATE_BOMB_QUESTION:
			drawBitmap(memDC, leftX, TopY, bombQuestion);
			break;

		case TILE_STATE_FAIL_FLAG:
			drawBitmap(memDC, leftX, TopY, failFlag);
			break;

		case TILE_STATE_OPEN:
			switch (Mine[i].tile_MineCount)
			{
			case 0:
				drawBitmap(memDC, leftX, TopY, tileMine0);
				break;

			case 1:
				drawBitmap(memDC, leftX, TopY, tileMine1);
				break;

			case 2:
				drawBitmap(memDC, leftX, TopY, tileMine2);
				break;

			case 3:
				drawBitmap(memDC, leftX, TopY, tileMine3);
				break;

			case 4:
				drawBitmap(memDC, leftX, TopY, tileMine4);
				break;

			case 5:
				drawBitmap(memDC, leftX, TopY, tileMine5);
				break;

			case 6:
				drawBitmap(memDC, leftX, TopY, tileMine6);
				break;

			case 7:
				drawBitmap(memDC, leftX, TopY, tileMine7);
				break;

			case 8:
				drawBitmap(memDC, leftX, TopY, tileMine8);
				break;
			}

			if (Mine[i].tile_mineExist)
			{
				drawBitmap(memDC, leftX, TopY, tileMine);
			}

			break;
		}
	}

	DeleteObject(tileFlag);
	DeleteObject(tileClose);
	DeleteObject(tileMine0);
	DeleteObject(tileMine1);
	DeleteObject(tileMine2);
	DeleteObject(tileMine3);
	DeleteObject(tileMine4);
	DeleteObject(tileMine5);
	DeleteObject(tileMine6);
	DeleteObject(tileMine7);
	DeleteObject(tileMine8);
	DeleteObject(tileQuestion);
	DeleteObject(tileMine);

	SelectObject(memDC, hOldBmp);
	DeleteObject(memDC);
	ReleaseDC(hWnd, hdc);

	return;
}


//������ �ʰų�, �߸� ������ Ÿ�ϵ� ������
void clearTiles()
{
	int nAllTileCount = currentTileWidth * currentTileHeight;

	for (int i = 0; i < nAllTileCount; i++)
	{
		if (Mine[i].tile_MineState == TILE_STATE_CLOSE)
		{
			Mine[i].tile_MineState = TILE_STATE_OPEN;
		}
		else if (Mine[i].tile_MineState == TILE_STATE_FLAG && !Mine[i].tile_mineExist)
		{
			Mine[i].tile_MineState = TILE_STATE_FAIL_FLAG;
		}
	}
}

//���� ���� == ���� Ÿ�� -> ����
void checkMines()
{
	int nAllTileCount = currentTileWidth * currentTileHeight;

	for (int i = 0; i < nAllTileCount; i++)
	{
		if (Mine[i].tile_MineState == TILE_STATE_CLOSE)
		{
			Mine[i].tile_MineState = TILE_STATE_FLAG;
			remainMine--;
		}
	}

	for (int i = 0; i < nAllTileCount; i++)
	{
		if (Mine[i].tile_mineExist)
		{
			if (Mine[i].tile_MineState != TILE_STATE_FLAG)
			{
				MessageBox(hWnd, L"Mission Failed", L"Game Over", MB_OK);
				Mine[i].tile_MineState = TILE_STATE_BOMB_MINE;
				gameState = GAME_OVER;
				clearTiles();

				return;
			}
		}
	}

	MessageBox(hWnd, L"congratulation", L"Victory", MB_OK);
	gameState = GAME_OVER;
	clearTiles();

	return;
}