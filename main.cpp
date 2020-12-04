#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("VladApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Lesson 12 exersize 2");


struct Image
{
	HBITMAP hBitmap;
	LONG width, height;
};

static HDC hdcMemSurface;
static struct Image images[3];

static struct Image loadImage(LPCTSTR path, HINSTANCE hInstance)
{
	struct Image image;

	image.hBitmap = (HBITMAP)LoadImage(
		hInstance,
		path,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	BITMAP bitmapInfo;
	GetObject(image.hBitmap, sizeof(bitmapInfo), &bitmapInfo);
	image.width = bitmapInfo.bmWidth;
	image.height = bitmapInfo.bmHeight;

	return image;
}

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = (HICON)LoadImage(NULL, TEXT("DAicon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = (HICON)LoadImage(NULL, TEXT("small_icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Programm"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_SYSMENU, // was: WS_OVERLAPPEDWINDOW
		CW_USEDEFAULT, CW_USEDEFAULT,
		1200, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Programm"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

int StrToInt(TCHAR* str) {
	int number = 0;
	int i = 0;
	if (str[i] == '-')
	{
		return -1;
	}
	while (str[i] >= 0x30 && str[i] <= 0x39)
	{
		number = number + (str[i] & 0x0F);
		number = number * 10;
		i++;
	}
	if (number == 0) {
		return -1;
	}
	number = number / 10;
	return number;
}

int IntToStr(int number) {
	return 1;
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR errorMsg[] = _T("Ошибка. Недопустимое значение высоты стены.");
	static LPCWSTR namenewwnd = L"Введите высоту стены: (число м)";
	static HWND hInfo;
	static HWND hEdt1;
	static HWND hBtn, rptBtn, quitBtn;

	switch (message)
	{
	case WM_CREATE:
	{
		//hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hEdt1 = CreateWindow(L"edit", namenewwnd,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 400, 200, 260, 20,
			hWnd, 0, hInst, NULL);
		ShowWindow(hEdt1, SW_SHOWNORMAL);

		hBtn = CreateWindow(L"button", L"Oк",
			WS_CHILD | WS_VISIBLE | WS_BORDER, 450, 250, 120, 30,
			hWnd, 0, hInst, NULL);
		ShowWindow(hBtn, SW_SHOWNORMAL);

		const HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE);

		images[0] = loadImage(TEXT("gun.bmp"), hInstance);
		images[1] = loadImage(TEXT("ball.bmp"), hInstance);
		images[2] = loadImage(TEXT("pr_ball.bmp"), hInstance);

		const HDC hdcWin = GetDC(hWnd);
		hdcMemSurface = CreateCompatibleDC(hdcWin);
		ReleaseDC(hWnd, hdcWin);

		return 0;
	}
	case WM_PAINT:
	{
		return 0;
	}
	case WM_COMMAND:
	{
		int Len, a, height, yBall = 300, xBall = 25, preY;
		bool hit = FALSE;
		TCHAR StrA[20];
		if (lParam == (LPARAM)hBtn)    // если нажали на кнопку
		{
			Len = GetWindowText(hEdt1, StrA, 20);
			a = StrToInt(StrA);
			DestroyWindow(hEdt1);
			DestroyWindow(hBtn);
			if (a == -1) {
				hdc = BeginPaint(hWnd, &ps);
				TextOut(hdc, 400, 220, errorMsg, _tcslen(errorMsg));
				EndPaint(hWnd, &ps);
				/*rptBtn = CreateWindow(L"button", L"Попробовать снова",
					WS_CHILD | WS_VISIBLE | WS_BORDER, 450, 250, 220, 30,
					hWnd, 0, hInst, NULL);
				ShowWindow(rptBtn, SW_SHOWNORMAL);*/
			}
			if (a >= 70) {
				hit = TRUE;
			}
			height = 300 - a * 2;

			hdc = BeginPaint(hWnd, &ps);
			SelectObject(hdcMemSurface, images[0].hBitmap);
			BitBlt(
				hdc,
				0, 310, // так просто выглядит лучше, отсчет все равно от 300
				images[0].width, images[0].height,
				hdcMemSurface,
				0, 0,
				SRCCOPY
			);

			Line(hdc, 25, 300, 1025, 300);
			Line(hdc, 425, 300, 425, height);

			SelectObject(hdcMemSurface, images[1].hBitmap);
			BitBlt(
				hdc,
				xBall, yBall,
				images[1].width, images[1].height,
				hdcMemSurface,
				0, 0,
				SRCCOPY
			);
			for (int i = 5; i < 309; i += 5) {
				SelectObject(hdcMemSurface, images[2].hBitmap);
				BitBlt(
					hdc,
					xBall, yBall,
					images[2].width, images[2].height,
					hdcMemSurface,
					0, 0,
					SRCCOPY
				);
				RECT R = { xBall, yBall, xBall + 10, yBall + 10 };
				InvalidateRect(hWnd, &R, TRUE);
				preY = i - 0.00324297 * i * i;
				yBall = 300 - preY * 2;
				xBall = i * 2 + 25;
				SelectObject(hdcMemSurface, images[1].hBitmap);
				BitBlt(
					hdc,
					xBall, yBall,
					images[1].width, images[1].height,
					hdcMemSurface,
					0, 0,
					SRCCOPY
				);
				if (i == 200) {
					if (hit) {
						hInfo = CreateWindow(
							szWindowClass, L"Information",
							WS_SYSMENU | WS_VISIBLE | WS_BORDER, 400, 400,
							300, 250,
							NULL, NULL, hInst, NULL
						);
						hdc = BeginPaint(hInfo, &ps);
						TextOut(hdc, 30, 45, L"Снаряд попал в стену.", 22);
						EndPaint(hInfo, &ps);
						quitBtn = CreateWindow(L"button", L"Oк",
							WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 125, 120, 30,
							hInfo, 0, hInst, NULL);
						ShowWindow(quitBtn, SW_SHOWNORMAL);
						UpdateWindow(quitBtn);
						/*hBtn = CreateWindow(L"static", L"0",
							WS_CHILD | WS_VISIBLE | WS_BORDER, 110, 50, 120, 30,
							hInfo, 0, hInst, NULL);
						ShowWindow(hBtn, SW_SHOWNORMAL);*/
						return 0;
					}
				}
				Sleep(90);
			}
			// окно о времени-скорости бла-бла..
			hInfo = CreateWindow(
				szWindowClass, L"Information",
				WS_SYSMENU | WS_VISIBLE | WS_BORDER, 400, 400,
				300, 250,
				NULL, NULL, hInst, NULL
			);
			hdc = BeginPaint(hInfo, &ps);
			TextOut(hdc, 15, 20, L"Снаряд не попал в стену.", 25);
			TextOut(hdc, 15, 40, L"Время полета: 2.88с", 20); // я это все сам посчитал просто~
			TextOut(hdc, 15, 60, L"Дальность полета: 308,46м.", 27); // можно формулы вписать в код, чтобы он считал
			TextOut(hdc, 15, 80, L"Максимальная высота: ~77м.", 27); // ...но уже как-то незачем
			EndPaint(hInfo, &ps);
			quitBtn = CreateWindow(L"button", L"Oк",
				WS_CHILD | WS_VISIBLE | WS_BORDER, 90, 125, 120, 30,
				hInfo, 0, hInst, NULL);
			ShowWindow(quitBtn, SW_SHOWNORMAL);
			UpdateWindow(quitBtn);
			EndPaint(hWnd, &ps);
		}
		else if (lParam == (LPARAM)quitBtn) {
			PostQuitMessage(0);
			return 0;
		}
		/*else if (lParam == (LPARAM)rptBtn) {
			DestroyWindow(rptBtn);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			hEdt1 = CreateWindow(L"edit", namenewwnd,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 400, 200, 260, 20,
				hWnd, 0, hInst, NULL);
			ShowWindow(hEdt1, SW_SHOWNORMAL);

			hBtn = CreateWindow(L"button", L"Oк",
				WS_CHILD | WS_VISIBLE | WS_BORDER, 450, 250, 120, 30,
				hWnd, 0, hInst, NULL);
			ShowWindow(hBtn, SW_SHOWNORMAL);

			const HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

			images[0] = loadImage(TEXT("gun.bmp"), hInstance);
			images[1] = loadImage(TEXT("ball.bmp"), hInstance);

			const HDC hdcWin = GetDC(hWnd);
			hdcMemSurface = CreateCompatibleDC(hdcWin);
			ReleaseDC(hWnd, hdcWin);
		}*/
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
		return 0;
	}
	}

	return 0;
}