#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Global Variables:															 Глобальные переменные:
HINSTANCE hInst;                                // current instance              Текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text            Текст заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name    Имя класса главного окна 

// Forward declarations of functions included in this code module:               Предварительные объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);   
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.          TODO: Разместить код здесь.

	// Initialize global strings            Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_POPCORNDOSGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:   Выполняем инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORNDOSGAME));

	MSG msg;

	// Main message loop:    Основной цикл сообщений:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION: MyRegisterClass()              ФУНКЦИЯ: MyRegisterClass()
//  PURPOSE: Registers the window class.     НАЗНАЧЕНИЕ: Регистрирует класс окна.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
   WNDCLASSEXW wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORNDOSGAME));
   wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));								//(HBRUSH)(COLOR_WINDOW + 1);
   wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORNDOSGAME);
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
  
   return RegisterClassExW(&wcex);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//   FUNCTION: InitInstance(HINSTANCE, int)                                         ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window                         НАЗНАЧЕНИЕ: Сохраняет дескриптор экземпляра и создает главное окно
// 
//   In this function, we save the instance handle in a global variable and         В этой функции мы сохраняем дескриптор экземпляра в глобальной переменной и
//   create and display the main program window.                                    создаем и отображаем главное окно программы.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable				Сохраняем дескриптор экземпляра в нашей глобальной переменной

   RECT window_rect;																// Создаем пользовательский размер окна для разных систем, где меню игры может различаться
   window_rect.left = 0;
   window_rect.top = 0;
   window_rect.right = 1265;
   window_rect.bottom = 633;

   AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

   //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
	   0, 0, window_rect.right-window_rect.left, window_rect.bottom-window_rect.top, nullptr, nullptr, hInstance, nullptr);

   if (hWnd == 0) return FALSE;
  
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{//	Отрисовка экрана игры

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)									ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE: Processes messages for the main window.								НАЗНАЧЕНИЕ: Обработка сообщений для главного окна.
//  WM_COMMAND  - process the application menu										WM_COMMAND - обработка меню приложения
//  WM_PAINT    - Paint the main window												WM_PAINT - Отрисовка главного окна
//  WM_DESTROY  - post a quit message and return									WM_DESTRUCTION - отправка сообщения о выходе и возврат
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:							 Анализируем пункты меню:
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


	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...	 TODO: Добавьте сюда любой код рисования, использующий hdc...
			Draw_Frame(hdc);
			EndPaint(hWnd, &ps);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Message handler for about box.									 Обработчик сообщений для поля «О нас».
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
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
