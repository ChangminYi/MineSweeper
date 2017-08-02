#include <Windows.h>

HINSTANCE g_hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DifficultyProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	static char szClassName[] = "����ã��";
	static char szTitle[] = "����ã��";

	MSG msg;
	HWND hWnd;
	WNDCLASSEX wc;
	g_hInst = hInstance;

	//���� ������ Ŭ����
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	//�� ���
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	//���� ������ Ŭ����
	wc.hCursor = LoadCursor(NULL, IDC_HELP);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "Difficulty";
	wc.lpfnWndProc = DifficultyProc;

	hWnd = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			CreateWindow("button", "�ʱ�", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 50, 200, 30, hWnd, (HMENU)1, g_hInst, NULL);
			CreateWindow("button", "�߱�", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 100, 200, 30, hWnd, (HMENU)2, g_hInst, NULL);
			CreateWindow("button", "���", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 150, 200, 30, hWnd, (HMENU)3, g_hInst, NULL);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case 1:
				{
					MessageBox(hWnd, "�ʱ� ����", "���̵�", MB_OK);
					break;
				}
				case 2:
				{
					MessageBox(hWnd, "�߱� ����", "���̵�", MB_OK);
					break;
				}
				case 3:
				{
					MessageBox(hWnd, "��� ����", "���̵�", MB_OK);
					break;
				}
			}
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

	return 0;
	}
}

LRESULT CALLBACK DifficultyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}