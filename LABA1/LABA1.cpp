﻿// LABA1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "LABA1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


HBITMAP hBitmap = NULL;
BITMAP Bitmap;
//HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Красная кисть для рисования овала
int xPos = 100;                                 // Начальная позиция X овала
int yPos = 100;                                 // Начальная позиция Y овала
bool isShiftPressed = false;                    // Флаг нажатия клавиши Shift


UINT_PTR nTimerID;
double xVelocity = 0.0;
double yVelocity = 0.0;
const double acceleration = 1.0;

RECT clientRect; // Клиентская область окна
SIZE spriteSize;  // Размеры спрайта

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LABA1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA1));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LABA1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void LoadBitmapFromFile()
{
  hBitmap = (HBITMAP)LoadImage(NULL, L"ball1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBitmap)
  {
    BITMAP bm;
    GetObject(hBitmap, sizeof(BITMAP), &bm);
    spriteSize.cx = bm.bmWidth;
    spriteSize.cy = bm.bmHeight;
  }
}
//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // Разобрать выбор в меню:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_SIZE:
  {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    // Устанавливаем минимальные размеры окна равными размерам спрайта
    if (clientRect.right - clientRect.left < spriteSize.cx || clientRect.bottom - clientRect.top < spriteSize.cy)
    {
      SetWindowPos(hWnd, NULL, 0, 0, spriteSize.cx, spriteSize.cy, SWP_NOMOVE | SWP_NOZORDER);
    }

    break;
  }
  case WM_CREATE:
    LoadBitmapFromFile();
    nTimerID = SetTimer(hWnd, 1, 10, NULL);
    break;

  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    if (hBitmap)
    {
      HDC hdcMem = CreateCompatibleDC(hdc);
      SelectObject(hdcMem, hBitmap);
      // Рисуем спрайт с учетом его размеров
      BitBlt(hdc, xPos, yPos, spriteSize.cx, spriteSize.cy, hdcMem, 0, 0, SRCCOPY);

      DeleteDC(hdcMem);
    }

    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    //DeleteObject(hBrush); // Освобождаем кисть
    KillTimer(hWnd, nTimerID);
    PostQuitMessage(0);
    break;

  case WM_MOUSEWHEEL:
  {
    int delta = GET_WHEEL_DELTA_WPARAM(wParam);
    GetClientRect(hWnd, &clientRect);

    if (delta > 0)
    {
      if (isShiftPressed)
      {
        xVelocity -= 1.0;
      }
      else
      {
        yVelocity -= 1.0;
      }
    }
    else
    {
      if (isShiftPressed)
      {
        xVelocity += 1.0;
      }
      else
      {
        yVelocity += 1.0;
      }
    }

    // Ограничьте скорость мячика, если она слишком большая
    const double maxSpeed = 4.0;
    if (xVelocity > maxSpeed) xVelocity = maxSpeed;
    if (xVelocity < -maxSpeed) xVelocity = -maxSpeed;
    if (yVelocity > maxSpeed) yVelocity = maxSpeed;
    if (yVelocity < -maxSpeed) yVelocity = -maxSpeed;

    // Запросите перерисовку окна
    //InvalidateRect(hWnd, NULL, TRUE);
  }
  break;

  case WM_LBUTTONUP:
  {
    // Сбросьте скорость мячика при отпускании левой кнопки мыши
    xVelocity = 0.0;
    yVelocity = 0.0;
  }
  break;

  // В функции WM_TIMER, измените позицию мячика на основе скорости
  case WM_TIMER:
  {
    // Обновите позицию мячика на основе скорости
    xPos +=(int)xVelocity;
    yPos +=(int)yVelocity;

    // Получите размеры клиентской области окна
    GetClientRect(hWnd, &clientRect);

    // Проверьте столкновение мячика с границами окна
    if (xPos < 0 || xPos + spriteSize.cx > clientRect.right)
    {
      // Мячик столкнулся с левой или правой границей, измените направление скорости по X
      xVelocity = -xVelocity * acceleration;
    }

    if (yPos < 0 || yPos + spriteSize.cy > clientRect.bottom)
    {
      // Мячик столкнулся с верхней или нижней границей, измените направление скорости по Y
      yVelocity = -yVelocity * acceleration;
    }

    // Запросите перерисовку окна
    InvalidateRect(hWnd, NULL, TRUE);
  }
  break;
  case WM_KEYDOWN:
    if (wParam == VK_SHIFT)
    {
      isShiftPressed = true; // Устанавливаем флаг нажатия Shift
    }
    break;
  case WM_KEYUP:
    if (wParam == VK_SHIFT)
    {
      isShiftPressed = false; // Сбрасываем флаг нажатия Shift
    }
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}


// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}