////////////////////////////////////////////////
////////////////////////////////////////////////
//헤더 파일
#include <Windows.h>
#include <time.h>
#include "resource.h"	//비트맵 사용을 위한 리소스 헤더파일 (resource.rc 참조)
#include "D:\Visual Studio 2017\지뢰찾기_SDK문제해결\Win32Project3\define_macro.h"	//매크로 헤더파일 (좀 꼬여서 절대경로로 찾음 -> 수정 시 상대경로로 바꿔서...)

////////////////////////////////////////////////
////////////////////////////////////////////////
/*함수들 (자세한 설명은 함수 정의 위에 써놓음)*/
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
//변수들
int remainMine;   //남아있는 지뢰 수.
int stageMine;	   //난이도 따라 설정한 지뢰 수
int closedTile;   //닫혀 있는 타일 수.  
int gameMode;	//게임 난이도
int gameState;	//게임 진행상황
int currentTileWidth;	//판 너비
int currentTileHeight;	//판 높이
double playTime;	//경과 시간
int background_L;	//판 좌변 좌표
int background_R;	//판 우변 좌표
int background_T;	//판 윗변 좌표
int background_B;	//판 아랫변 좌표
bool L_Pressed;	//마우스 좌클릭 확인용 변수
bool R_Pressed; //마우스 우클릭		"

//판 관련 배열 (구조체로 한 번에 선언)
typedef struct MINE_STATUS
{
int tile_MineCount;
int tile_MineState;
bool tile_mineExist;
}mine;
mine Mine[HARD_TILE_WIDTH * HARD_TILE_HEIGHT];

//인스턴스 핸들, 윈도우 핸들, 비트맵
HINSTANCE g_hInst;
HWND hWnd, hStartButton;
HBITMAP memoryBitmap;	// 가상 메모리 비트맵. 화면을 메모리에 그리고 난 후 WndProc: WM_PAINT에서 메모리 비트맵을 그림.


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//진입점
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

//WndProc 함수 (WinMain에서 받은 메시지 해석)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memDC;
	PAINTSTRUCT ps;
	RECT tempRect;
	HBITMAP hOldBmp;
	int mouseX, mouseY, index, indexRow, indexColumm, leftX, TopY;
	int nMatchMineCount, buttonWidth, buttonHeight;

	//메시지 처리용 switch문
	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &tempRect);
		setGameState(HARD_MODE, &tempRect);

		buttonWidth = 100;
		buttonHeight = 30;
		leftX = tempRect.right / 2; //가로 길이 중점
		TopY = 20;	//세로 길이 중점
		leftX -= (buttonWidth / 2);

		//변수 시드
		srand((int)time(NULL));

		hStartButton = CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leftX, TopY, buttonWidth, buttonHeight, hWnd, (HMENU)ID_START, g_hInst, NULL);
		L_Pressed = false;
		R_Pressed = false;

		//타이머
		SetTimer(hWnd, 1, 500, NULL);

		//화면 출력
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
			InvalidateRect(hWnd, NULL, FALSE);	//화면 다시 출력시키는 함수
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
				indexRow = (mouseY - background_T) / TILE_IMAGE_HEIGHT;	//index: 현재 칸의 번호
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

				gameState = GAME_PLAY;	//타이머 초기화, 모드 Play로 변경

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
//여기서부터 함수들 정의

//처음 게임 상태 초기화하는 함수
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

//비트맵 출력하는 함수
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

//지뢰 생성하는 함수
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

//타일이 열렸을 때 실행되는 체크함수
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

//열린 타일 체크하는 함수
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

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)	//좌상향
	{
		iCurrTileState = Mine[index - currentTileWidth - 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol - 1);
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)	//중상향
	{
		iCurrTileState = Mine[index - currentTileWidth].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol);
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)	//우상향
	{
		iCurrTileState = Mine[index - currentTileWidth + 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow - 1, indexCol + 1);
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)	//좌중향
	{
		iCurrTileState = Mine[index - 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow, indexCol - 1);
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)	//우중향
	{
		iCurrTileState = Mine[index + 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow, indexCol + 1);
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)	//좌하향
	{
		iCurrTileState = Mine[index + currentTileWidth - 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol - 1);
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)	//중하향
	{
		iCurrTileState = Mine[index + currentTileWidth].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol);
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)	//우하향
	{
		iCurrTileState = Mine[index + currentTileWidth + 1].tile_MineState;
		if (iCurrTileState == TILE_STATE_CLOSE || iCurrTileState == TILE_STATE_QUESTION)
		{
			checkTileState(indexRow + 1, indexCol + 1);
		}
	}

	return;
}


//지뢰 수 세는 함수
int getAdjustMineCount(int indexRow, int indexCol)
{
	int MineCount = 0;
	int index = (indexRow * currentTileWidth) + indexCol;

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)	//좌상향
	{
		if (Mine[index - currentTileWidth - 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)	//중상향
	{
		if (Mine[index - currentTileWidth].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)	//우상향
	{
		if (Mine[index - currentTileWidth + 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)	//좌중향
	{
		if (Mine[index - 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)	//우중향
	{
		if (Mine[index + 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)	//좌하향
	{
		if (Mine[index + currentTileWidth - 1].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)	//중하향
	{
		if (Mine[index + currentTileWidth].tile_mineExist)
		{
			MineCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)	//우하향
	{
		if (Mine[index + currentTileWidth + 1].tile_mineExist)
		{
			MineCount++;
		}
	}

	return MineCount;
}

//깃발 세는 함수
int getAdjustFlagCount(int indexRow, int indexCol)
{
	int FlagCount = 0;
	int index = (indexRow * currentTileWidth) + indexCol;

	if (indexCol - 1 >= 0 && indexRow - 1 >= 0)	//좌상향
	{
		if (Mine[index-currentTileWidth-1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol >= 0 && indexRow - 1 >= 0)	//중상향
	{
		if (Mine[index-currentTileWidth].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow - 1 >= 0)	//우상향
	{
		if (Mine[index-currentTileWidth+1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow >= 0)	//좌중향
	{
		if (Mine[index-1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow >= 0)	//우중향
	{
		if (Mine[index+1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol - 1 >= 0 && indexRow + 1 < currentTileHeight)	//좌하향
	{
		if (Mine[index+currentTileWidth-1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol >= 0 && indexRow + 1 < currentTileHeight)	//중하향
	{
		if (Mine[index+currentTileWidth].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}
	if (indexCol + 1 < currentTileWidth && indexRow + 1 < currentTileHeight)	//우하향
	{
		if (Mine[index+currentTileWidth+1].tile_MineState == TILE_STATE_FLAG)
		{
			FlagCount++;
		}
	}

	return FlagCount;
}

//숫자(시간) 출력하는 함수
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

//판 사이즈 결정하는 함수
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

//전체 화면 출력하는 함수
void printMineSweeper()
{
	HDC hdc, memDC;
	HBITMAP tileFlag, tileClose, tileMine0, tileMine1, tileMine2, tileMine3, tileMine4, tileMine5, tileMine6, tileMine7, tileMine8, tileQuestion, tileMine, failFlag, bombMine, bombQuestion, hOldBmp, numMinus;
	int index, indexRow, indexColumm, leftX, TopY, nAllTileCount;

	if (memoryBitmap)	//이미 출력된 화면 지움
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


//열리지 않거나, 잘못 설정된 타일들 보여줌
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

//남은 지뢰 == 남은 타일 -> 성공
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