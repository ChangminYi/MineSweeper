#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "resource.h"

#define ID_START 201
#define ID_GAME_NEWGAME 40001
#define ID_GAME_EXIT 40002
#define ID_MODE_EASY 40003
#define ID_MODE_MIDDLE 40004
#define ID_MODE_HARD 40005

//지뢰 상태 나타내는 부분
#define TILE_STATE_CLOSE 0
#define TILE_STATE_QUESTION 1
#define TILE_STATE_FLAG 2
#define TILE_STATE_OPEN 3
#define TILE_STATE_FAIL 4
#define TILE_STATE_BOMB_MINE 5
#define TILE_STATE_BOMB_QUESTION 6

//판 구성요소 크기
#define NUMBER_IMAGE_WIDTH 14
#define NUMBER_IMAGE_HEIGHT 20
#define TILE_IMAGE_WIDTH 20
#define TILE_IMAGE_HEIGHT 20

//난이도
#define EASY 0
#define MIDDLE 1
#define HARD 2

//난이도 따른 판 크기
#define EASY_WIDTH 9
#define EASY_HEIGHT 9
#define MIDDLE_WIDTH 16
#define MIDDLE_HEIGHT 16
#define HARD_WIDTH 30
#define HARD_HEIGHT 16

//지뢰갯수
#define EASY_MINE 10
#define MIDDLE_MINE 40
#define HARD_MINE 99

//게임 진행 상태
#define GAME_READY 0
#define GAME_PLAYING 1
#define GAME_END 2

//게임 진행상황 알리는 변수들
int remainMine;
int stageMine;
int closedTile;

//전체 화면 크기
int backgroundLeft;
int backgroundRight;
int backgroundTop;
int backgroundBottom;

//게임 관련 배열들(구조체로 변경해도 될 듯)
int tileAdjustMineCount[HARD_WIDTH*HARD_HEIGHT];
int tileMineState[HARD_WIDTH*HARD_HEIGHT];
bool mineExist[HARD_WIDTH*HARD_HEIGHT];

int gameMode;
int gameState;

int currentTileWidth;
int currentTileHeight;

double playTime;

bool pressedLButton;
bool pressedRButton;

HINSTANCE g_hInst;
HWND hWnd, hStartBtn;
HBITMAP memBit;
PAINTSTRUCT ps;

//함수들
void setBackgroundSize(LPRECT clientRect);
void setGameState(int gameMod, LPRECT clientRect);
void generateMine(int tileIndex);

void checkTileState(int indexRow, int indexCol);
void checkOpenTile(int indexRow, int indexCol);
void checkMine();
void clearTile();

int getAdjustMineCount(int indexRow, int indexCol);
int getAdjustFlagCount(int indexRow, int indexCol);

//화면출력
void drawBitmap(HDC hdc, int x, int y, HBITMAP hBitmap);
void drawNumber(HDC hdc, int x, int y, int drawNumber);
void printMineFinder();

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int showCmd)
{
	LPSTR lpszClass = "MyButton";
	WNDCLASSEX wc;
	MSG msg;

	g_hInst = hInstance;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Window";
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hWnd = CreateWindow(L"WINDOW", L"WINDOW", WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, showCmd);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memDC;
	PAINTSTRUCT ps;
	int mouseX, mouseY, index, indexRow, indexCol, leftX, topY;
	int matchMineCount, buttonWidth, buttonHeight;
	RECT tempRect;
	HBITMAP hOldBmp;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &tempRect);
		setGameState(HARD, &tempRect);

		buttonWidth = 100;
		buttonHeight = 30;
		leftX = (tempRect.right / 2);
		topY = 20;
		leftX -= (buttonWidth / 2);

		srand((int)time(NULL));
		hStartBtn = CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftX, topY, buttonWidth, buttonHeight, hWnd, (HMENU)ID_START, g_hInst, NULL);

		pressedLButton = false;
		pressedRButton = false;

		SetTimer(hWnd, 1, 500, NULL);

		printMineFinder();

		break;

	case WM_TIMER:
		if (gameState == GAME_PLAYING)
		{
			playTime += 0.5f;

			if (playTime > 999.0f)
			{
				playTime = 999.0f;
			}

			printMineFinder();
			InvalidateRect(hWnd, NULL, FALSE);
		}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_START:
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineFinder();

			InvalidateRect(hWnd, NULL, FALSE);

			break;
		case ID_GAME_NEWGAME:
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineFinder();
			InvalidateRect(hWnd, NULL, FALSE);

			break;
		case ID_MODE_EASY:
			gameMode = EASY;
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineFinder();
			InvalidateRect(hWnd, NULL, TRUE);
		case ID_MODE_MIDDLE:
			gameMode = MIDDLE;
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineFinder();
			InvalidateRect(hWnd, NULL, TRUE);

			break;
		case ID_MODE_HARD:
			gameMode = HARD;
			GetClientRect(hWnd, &tempRect);
			setGameState(gameMode, &tempRect);

			printMineFinder();
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
		leftX = (tempRect.right / 2);
		topY = 20;

		leftX -= (buttonWidth / 2);

		MoveWindow(hStartBtn, leftX, topY, buttonWidth, buttonHeight, TRUE);

		printMineFinder();
		InvalidateRect(hWnd, NULL, TRUE);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hdc);

		hOldBmp = (HBITMAP)SelectObject(memDC, memBit);
		BitBlt(hdc, 0, 0, backgroundRight, backgroundBottom, memDC, 0, 0, SRCCOPY);
		SelectObject(memDC, hOldBmp);
		DeleteDC(memDC);
		EndPaint(hWnd, &ps);

		return 0;
		break;
	case WM_LBUTTONDOWN:
		pressedLButton = true;
		if (pressedRButton)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);

			if (mouseX > backgroundLeft && mouseX<backgroundRight && mouseY>backgroundTop && mouseY < backgroundBottom)
			{
				indexCol = (mouseX - backgroundLeft) / TILE_IMAGE_WIDTH;
				indexRow = (mouseY - backgroundTop) / TILE_IMAGE_HEIGHT;

				index = indexRow*currentTileWidth + indexCol;

				if (gameState == GAME_PLAYING)
				{
					if (tileMineState[index] == TILE_STATE_OPEN)
					{
						checkOpenTile(indexRow, indexCol);
					}
				}

				printMineFinder();
				InvalidateRect(hWnd, NULL, FALSE);
			}

			break;
	case WM_RBUTTONDOWN:
		pressedRButton = true;

		if (pressedLButton)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);

			if (mouseX > backgroundLeft && mouseX<backgroundRight && mouseY>backgroundTop && mouseY < backgroundBottom)
			{
				indexCol = (mouseX - backgroundLeft) / TILE_IMAGE_WIDTH;
				indexRow = (mouseY - backgroundTop) / TILE_IMAGE_HEIGHT;

				index = indexRow*currentTileWidth + indexCol;

				if (gameState == GAME_PLAYING)
				{
					if (tileMineState[index] == TILE_STATE_OPEN)
					{
						checkOpenTile(indexRow, indexCol);
					}
				}

				printMineFinder();
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}

		break;
	case WM_LBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		if (mouseX > backgroundLeft && mouseX<backgroundRight && mouseY>backgroundTop && mouseY < backgroundBottom)
		{
			indexCol = (mouseX - backgroundLeft) / TILE_IMAGE_WIDTH;
			indexRow = (mouseY - backgroundTop) / TILE_IMAGE_HEIGHT;

			index = indexRow*currentTileWidth + indexCol;

			switch (gameState)
			{
			case GAME_READY:
				generateMine(index);
				checkTileState(indexRow, indexCol);

				gameState = GAME_PLAYING;

				break;
			case GAME_PLAYING:
				if (tileMineState[index] == TILE_STATE_CLOSE)
				{
					if (mineExist[index] == true)
					{
						tileMineState[index] = TILE_STATE_BOMB_MINE;
						gameState = GAME_END;
						MessageBox(hWnd, L"Game Over", L"Game Over", MB_OK);

						clearTile();
					}
					else
					{
						checkTileState(indexRow, indexCol);
					}
				}
				if (closedTile == remainMine)
				{
					checkMine();
				}

				break;
			}
		}
		pressedLButton = false;
		printMineFinder();
		InvalidateRect(hWnd, NULL, FALSE);

		break;
	case WM_RBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		matchMineCount = 0;
		if (mouseX > backgroundLeft && mouseX<backgroundRight && mouseY>backgroundTop && mouseY < backgroundBottom)
		{
			indexCol = (mouseX - backgroundLeft) / TILE_IMAGE_WIDTH;
			indexRow = (mouseY - backgroundTop) / TILE_IMAGE_HEIGHT;

			index = indexRow*currentTileWidth + indexCol;

			if (gameState == GAME_PLAYING)
			{
				if (tileMineState[index] == TILE_STATE_CLOSE)
				{
					tileMineState[index] = TILE_STATE_FLAG;
					closedTile--;
					remainMine--;
				}
				else if (tileMineState[index] == TILE_STATE_FLAG)
				{
					remainMine++;
					closedTile++;

					tileMineState[index] = TILE_STATE_QUESTION;
				}
				else if (tileMineState[index] == TILE_STATE_QUESTION)
				{
					tileMineState[index] = TILE_STATE_CLOSE;
				}
			}
		}

		pressedLButton = false;
		printMineFinder();
		InvalidateRect(hWnd, NULL, FALSE);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

//밑에서부터 user-defined func.

void setBackgroundSize(LPRECT clientRect)
{
	int imageWidthGap, imageHeightGap;

	switch (gameMode)
	{
	case EASY:
		imageWidthGap = ((clientRect->right - clientRect->left) - (TILE_IMAGE_WIDTH*EASY_WIDTH));
		imageHeightGap = ((clientRect->bottom - clientRect->top) - (TILE_IMAGE_HEIGHT*EASY_HEIGHT));
		remainMine = EASY_MINE;
		currentTileWidth = EASY_WIDTH;
		currentTileHeight = EASY_HEIGHT;

		break;
	case MIDDLE:
		imageWidthGap = ((clientRect->right - clientRect->left) - (TILE_IMAGE_WIDTH*MIDDLE_WIDTH));
		imageHeightGap = ((clientRect->bottom - clientRect->top) - (TILE_IMAGE_HEIGHT*MIDDLE_HEIGHT));
		remainMine = MIDDLE_MINE;
		currentTileWidth = MIDDLE_WIDTH;
		currentTileHeight = MIDDLE_HEIGHT;

		break;
	case HARD:
		imageWidthGap = ((clientRect->right - clientRect->left) - (TILE_IMAGE_WIDTH*HARD_WIDTH));
		imageHeightGap = ((clientRect->bottom - clientRect->top) - (TILE_IMAGE_HEIGHT*HARD_HEIGHT));
		remainMine = HARD_MINE;
		currentTileWidth = HARD_WIDTH;
		currentTileHeight = HARD_HEIGHT;

		break;
	}
	stageMine = remainMine;
	backgroundLeft = imageWidthGap + clientRect->left;
	backgroundRight = clientRect->right - imageWidthGap;
	backgroundTop = imageHeightGap + clientRect->top;
	backgroundBottom = clientRect->bottom - imageHeightGap;
}

void setGameState(int gameMod, LPRECT clientRect)
{
	int allTileCount = HARD_WIDTH*HARD_HEIGHT;

	gameMode = gameMod;
	gameState = GAME_READY;
	playTime = 0.0;
	setBackgroundSize(clientRect);

	for (int i = 0; i < allTileCount; i++)
	{
		tileAdjustMineCount[i] = 0;
		tileMineState[i] = TILE_STATE_CLOSE;
		mineExist[i] = false;
	}

	return;
}

void generateMine(int tileIndex)
{
	int generatedMine = 0;
	int tmpIndex, tmpMineCount, allTileCount;
	int indexRow, indexCol;

	switch (gameMode)
	{
	case EASY:
		allTileCount = EASY_HEIGHT*EASY_WIDTH;
		break;
	case MIDDLE:
		allTileCount = MIDDLE_HEIGHT*MIDDLE_WIDTH;
		break;
	case HARD:
		allTileCount = HARD_HEIGHT*HARD_WIDTH;
		break;
	}

	closedTile = allTileCount;
	while (generatedMine != remainMine)
	{
		tmpIndex = rand() % allTileCount;
		if (tmpIndex != tileIndex && mineExist[tmpIndex])
		{
			mineExist[tmpIndex] = true;
			generatedMine++;
		}
	}
	for (int i = 0; i < allTileCount; i++)
	{
		indexRow = i / currentTileWidth;
		indexCol = i%currentTileWidth;
		tmpMineCount = getAdjustMineCount(indexRow, indexCol);
		tileAdjustMineCount[i] = tmpMineCount;
	}
}

void checkTileState(int indexRow, int indexCol)
{
	int allTileCount, prevTileState;
	int index;

	switch (gameMode)
	{
	case EASY:
		allTileCount = EASY_HEIGHT*EASY_WIDTH;
		break;
	case MIDDLE:
		allTileCount = MIDDLE_WIDTH*MIDDLE_HEIGHT;
		break;
	case HARD:
		allTileCount = HARD_WIDTH*HARD_HEIGHT;
		break;
	}

	index = (indexRow*currentTileWidth) + indexCol;
	prevTileState = tileMineState[index];

	if (!(prevTileState == TILE_STATE_CLOSE || prevTileState == TILE_STATE_QUESTION))
	{
		return;
	}
	if (indexRow < 0 || indexRow >= currentTileHeight || indexCol < 0 || indexCol >= currentTileWidth)
	{
		return;
	}

	tileMineState[index] = TILE_STATE_OPEN;
	closedTile--;

	if (mineExist[index])
	{
		gameState = GAME_END;
		if (prevTileState = TILE_STATE_BOMB_QUESTION)
		{
			tileMineState[index] = TILE_STATE_BOMB_QUESTION;
		}
		else
		{
			tileMineState[index] = TILE_STATE_BOMB_MINE;
		}

		clearTile();
		MessageBox(hWnd, L"Failed", L"Game Over", MB_OK);
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
}

void checkOpenTile(int indexRow, int indexCol)
{
	int allTileCount, index, currentTileState;

	switch (gameMode)
	{
	case EASY:
		allTileCount = EASY_WIDTH*EASY_HEIGHT;
		break;
	case MIDDLE:
		allTileCount = MIDDLE_WIDTH*MIDDLE_HEIGHT;
		break;
	case HARD:
		allTileCount = HARD_WIDTH*HARD_HEIGHT;
		break;
	}

	index = (indexRow*currentTileWidth) + indexCol;

	if (tileMineState[index] != TILE_STATE_OPEN)
	{
		return;
	}
	if (indexRow < 0 || indexRow >= currentTileHeight || indexCol < 0 || indexCol >= currentTileWidth)
	{
		return;
	}
	if (getAdjustFlagCount(indexRow, indexCol) != tileAdjustMineCount[index])
	{
		return;
	}

	if (indexCol >= 1 && indexRow >= 1)
	{
		currentTileState = tileMineState[index - currentTileWidth - 1];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol - 1);
		}
	}
	if (indexCol >= 0 && indexRow >= 1)
	{
		currentTileState = tileMineState[index - currentTileWidth];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol);
		}
	}
	if (indexCol < currentTileWidth - 1 && indexRow >= 1)
	{
		currentTileState = tileMineState[index - currentTileWidth + 1];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol + 1);
		}
	}
	if (indexCol >= 1 && indexRow >= 0)
	{
		currentTileState = tileMineState[index - 1];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow, indexCol - 1);
		}
	}
	if (indexCol < currentTileWidth - 1 && indexRow >= 0)
	{
		currentTileState = tileMineState[index + 1];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow, indexCol + 1);

		}
	}
	if (indexCol >= 1 && indexRow < currentTileHeight - 1)
	{
		currentTileState = tileMineState[index + currentTileWidth - 1];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol - 1);
		}
	}
	if (indexCol >= 0 && indexRow < currentTileHeight - 1)
	{
		currentTileState = tileMineState[index + currentTileWidth];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol);
		}
	}
	if (indexCol < currentTileWidth - 1 && indexRow < currentTileHeight - 1)
	{
		currentTileState = tileMineState[index + currentTileWidth + 1];
		if (currentTileState == TILE_STATE_CLOSE || currentTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol + 1);
		}
	}
}

void checkMine()
{
	int allTileCount = currentTileWidth*currentTileHeight;

	for (int i = 0; i < allTileCount; i++)
	{
		if (tileMineState[i] == TILE_STATE_CLOSE)
		{
			tileMineState[i] = TILE_STATE_FLAG;
			remainMine--;
		}
	}

	for (int i = 0; i < allTileCount; i++)
	{
		if (mineExist[i])
		{
			if (tileMineState[i] != TILE_STATE_FLAG)
			{
				MessageBox(hWnd, L"Failed", L"Game Over", MB_OK);

				tileMineState[i] = TILE_STATE_BOMB_MINE;
				gameState = GAME_END;
				clearTile();

				return;
			}
		}
	}

	MessageBox(hWnd, L"Congratulation", L"Victory", MB_OK);

	gameState = GAME_END;
	clearTile();
}

void clearTile()
{
	int allTileCount = currentTileWidth*currentTileHeight;

	for (int i = 0; i < allTileCount; i++)
	{
		if (tileMineState[i] == TILE_STATE_CLOSE)
		{
			tileMineState[i] = TILE_STATE_OPEN;
		}
		else if (tileMineState[i] == TILE_STATE_FLAG && !mineExist[i])
		{
			tileMineState[i] = TILE_STATE_FAIL;
		}
	}
}

int getAdjustMineCount(int indexRow, int indexCol)
{
	int mineCount = 0;
	int index = (indexRow*currentTileWidth) + indexCol;

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)
	{
		if (mineExist[index - currentTileWidth - 1])
		{
			mineCount++;
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)
	{
		if (mineExist[index - currentTileWidth])
		{
			mineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)
	{
		if (mineExist[index - currentTileWidth + 1])
		{
			mineCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)
	{
		if (mineExist[index - 1])
		{
			mineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)
	{
		if (mineExist[index + 1])
		{
			mineCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)
	{
		if (mineExist[index + currentTileWidth - 1])
		{
			mineCount++;
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)
	{
		if (mineExist[index + currentTileWidth])
		{
			mineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)
	{
		if (mineExist[index + currentTileWidth + 1])
		{
			mineCount++;
		}
	}

	return mineCount;
}

int getAdjustFlagCount(int indexRow, int indexCol)
{
	int flagCount = 0;
	int index = (indexRow * currentTileWidth) + indexCol;

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)
	{
		if (tileMineState[index - currentTileWidth - 1] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)
	{
		if (tileMineState[index - currentTileWidth] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)
	{
		if (tileMineState[index - currentTileWidth + 1] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)
	{
		if (tileMineState[index - 1] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)
	{
		if (tileMineState[index + 1] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)
	{
		if (tileMineState[index + currentTileWidth - 1] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)
	{
		if (tileMineState[index + currentTileWidth] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)
	{
		if (tileMineState[index + currentTileWidth + 1] == TILE_STATE_FLAG)
		{
			flagCount++;
		}
	}

	return flagCount;
}

void drawBitmap(HDC hdc, int x, int y, HBITMAP hBitmap)
{
	HDC memDC;
	HBITMAP oldBitmap;
	int imgHeight, imgWidth;
	BITMAP bit;

	memDC = CreateCompatibleDC(hdc);
	oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), &bit);
	imgWidth = bit.bmWidth;
	imgHeight = bit.bmHeight;

	BitBlt(hdc, x, y, imgWidth, imgHeight, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBitmap);
	DeleteObject(oldBitmap);
	DeleteDC(memDC);
}

void drawNumber(HDC hdc, int x, int y, int drawNumber)
{
	HBITMAP nBitmap;

	switch (drawNumber)
	{
	case 0:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_0));
		break;
	case 1:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_1));
		break;
	case 2:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_2));
		break;
	case 3:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_3));
		break;
	case 4:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_4));
		break;
	case 5:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_5));
		break;
	case 6:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_6));
		break;
	case 7:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_7));
		break;
	case 8:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_8));
		break;
	case 9:
		nBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_9));
		break;
	}

	drawBitmap(hdc, x, y, nBitmap);
	DeleteObject(nBitmap);
}

void printMineFinder()
{
	HDC hdc, memDC;
	HBITMAP tileFlag, tileClose, tileMine0, tileMine1, tileMine2, tileMine3, tileMine4, tileMine5, tileMine6, tileMine7, tileMine8;
	HBITMAP tileQues, tileMine, failFlag, bombMine, bombQues, hOldBitmap, nMinus;
	int index, indexRow, indexCol, leftX, topY, allTileCount;

	if(memBit)
	{
		DeleteObject(memBit);
	}

	BeginPaint(hWnd, &ps);
	hdc = GetDC(hWnd);
	memBit = CreateCompatibleBitmap(hdc, backgroundRight, backgroundBottom);
	memDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(memDC, memBit);

	PatBlt(memDC, 0, 0, backgroundRight, backgroundBottom, PATCOPY);

	tileFlag = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLAG));
	tileClose = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLOSE));
	tileMine0 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_0));
	tileMine1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_1));
	tileMine2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_2));
	tileMine3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_3));
	tileMine4 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_4));
	tileMine5 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_5));
	tileMine6 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_6));
	tileMine7 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_7));
	tileMine8 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINE_8));
	tileQues = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_QUESTION));
	tileMine = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOMB_COLD));
	failFlag = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOMB_WRONG));
	bombMine = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOMB_EXPLODE));
	bombQues = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOMB_QUESTION));
	nMinus = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TIME_MINUS));

	switch(gameMode)
	{
	case EASY:
		allTileCount = EASY_HEIGHT*EASY_WIDTH;
		break;
	case MIDDLE:
		allTileCount = MIDDLE_HEIGHT*MIDDLE_WIDTH;
		break;
	case HARD:
		allTileCount = HARD_HEIGHT*HARD_WIDTH;
		break;
	}

	index = (int(playTime)) % 10;
	leftX = backgroundLeft + NUMBER_IMAGE_WIDTH * 3;
	topY = backgroundTop + NUMBER_IMAGE_HEIGHT * 2;
	drawNumber(memDC, leftX, topY, index);

	index = (int(playTime)) / 10;
	if(index==26)
	{
		index = index;
	}
	if(index>=10)
	{
		index %= 10;
	}

	leftX -= NUMBER_IMAGE_WIDTH;
	drawNumber(memDC, leftX, topY, index);

	index = (int(playTime)) / 100;
	leftX -= NUMBER_IMAGE_WIDTH;
	drawNumber(memDC, leftX, topY, index);

	index = abs(remainMine) % 10;
	leftX = backgroundRight - NUMBER_IMAGE_WIDTH * 2;
	topY = backgroundTop - NUMBER_IMAGE_HEIGHT * 2;
	drawNumber(memDC, leftX, topY, index);

	index = abs(remainMine) / 10;
	leftX -= NUMBER_IMAGE_WIDTH;
	drawNumber(memDC, leftX, topY, index);

	if(remainMine<0)
	{
		leftX -= NUMBER_IMAGE_WIDTH;
		drawBitmap(memDC, leftX, topY, nMinus);
	}

	for(int i=0;i<allTileCount;i++)
	{
		indexRow = i / currentTileWidth;
		indexCol = i%currentTileWidth;

		leftX = backgroundLeft + indexCol*TILE_IMAGE_WIDTH;
		topY = backgroundTop + indexRow*TILE_IMAGE_HEIGHT;

		switch (tileMineState[i])
		{
		case TILE_STATE_CLOSE:
			drawBitmap(memDC, leftX, topY, tileClose);
			break;
		case TILE_STATE_QUESTION:
			drawBitmap(memDC, leftX, topY, tileQues);
			break;
		case TILE_STATE_FLAG:
			drawBitmap(memDC, leftX, topY, tileFlag);
			break;
		case TILE_STATE_BOMB_MINE:
			drawBitmap(memDC, leftX, topY, bombMine);
			break;
		case TILE_STATE_BOMB_QUESTION:
			drawBitmap(memDC, leftX, topY, bombQues);
			break;
		case TILE_STATE_FAIL:
			drawBitmap(memDC, leftX, topY, failFlag);
			break;
		case TILE_STATE_OPEN:
			switch(tileAdjustMineCount[i])
			{
			case 0:
				drawBitmap(memDC, leftX, topY, tileMine0);
				break;
			case 1:
				drawBitmap(memDC, leftX, topY, tileMine1);
				break;
			case 2:
				drawBitmap(memDC, leftX, topY, tileMine2);
				break;
			case 3:
				drawBitmap(memDC, leftX, topY, tileMine3);
				break;
			case 4:
				drawBitmap(memDC, leftX, topY, tileMine4);
				break;
			case 5:
				drawBitmap(memDC, leftX, topY, tileMine5);
				break;
			case 6:
				drawBitmap(memDC, leftX, topY, tileMine6);
				break;
			case 7:
				drawBitmap(memDC, leftX, topY, tileMine7);
				break;
			case 8:
				drawBitmap(memDC, leftX, topY, tileMine8);
				break;
			}

			if(mineExist[i])
			{
				drawBitmap(memDC, leftX, topY, tileMine);
			}

			break;
		}
	}

	DeleteObject(tileMine);
	DeleteObject(tileQues);
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

	SelectObject(memDC, hOldBitmap);
	DeleteObject(memDC);

	ReleaseDC(hWnd, hdc);
	EndPaint(hWnd, &ps);
}
