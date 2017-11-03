// 1512651PaintBeginner.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512651PaintBeginner.h"
#include <windowsx.h>
#include <iostream>
#include <vector>
#include <WinUser.h>
#include <Wingdi.h>
#include <algorithm> 
//#include <afxwin.h>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_1512651PAINTBEGINNER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_1512651PAINTBEGINNER));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_1512651PAINTBEGINNER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_1512651PAINTBEGINNER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
//rect xac dinh diem tren trai va duoi phai cua hinh chu nhat
struct Shape {
	RECT rect;
	UINT ID;
};
HMENU hMenu;
const UINT SHAPE_COUNT = 3;
Shape Shapes[SHAPE_COUNT];
UINT currentIndex = 0;

bool  isRubberBand = false;
POINT startPoint;
POINT currentPoint;


LRESULT OnCommand(HWND hWnd, int iID, int iEvent, HWND hWndControl, bool &isHandled);
LRESULT OnLButtonDown(HWND hWnd, UINT nCtrl, UINT x, UINT y);
LRESULT OnMouseMove(HWND hWnd, UINT nCtrl, UINT x, UINT y);
LRESULT OnLButtonUp(HWND hWnd, UINT nCtrl, UINT x, UINT y);
LRESULT OnPaint(HWND hWnd);
void Draw(HWND hWnd);
UINT Flag;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		hMenu = GetMenu(hWnd);
		CheckMenuItem(hMenu, ID_SHAPE_LINE, MF_CHECKED);
		break;
	}
	case WM_COMMAND:
	{
		hMenu = ::GetMenu(hWnd);
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			/*case ID_SHAPE_ELLIPSE:
			Ellipse(hdc, 200, 200, 100, 100);*/
		case ID_SHAPE_LINE:
		{
			CheckMenuItem(hMenu, ID_SHAPE_LINE, MF_CHECKED);
			CheckMenuItem(hMenu, ID_SHAPE_RECTANGLE, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_SHAPE_ELLIPSE, MF_UNCHECKED);
			break;
		}
		case ID_SHAPE_RECTANGLE:
		{
			CheckMenuItem(hMenu, ID_SHAPE_LINE, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_SHAPE_RECTANGLE, MF_CHECKED);
			CheckMenuItem(hMenu, ID_SHAPE_ELLIPSE, MF_UNCHECKED);
			break;
		}
		case ID_SHAPE_ELLIPSE:
		{
			//C: Set the drawing mode.
			//::CheckMenuRadioItem(hMenuShapes, ID_SHAPE_RECTANGLE, ID_SHAPE_ELLIPSE, wmId, MF_BYCOMMAND);
			CheckMenuItem(hMenu, ID_SHAPE_LINE, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_SHAPE_RECTANGLE, MF_UNCHECKED);
			CheckMenuItem(hMenu, ID_SHAPE_ELLIPSE, MF_CHECKED);
			break;
		}
		break;
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
	case WM_PAINT:	OnPaint(hWnd);
		break;
	case WM_MOUSEMOVE: OnMouseMove(hWnd, (UINT)wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN: OnLButtonDown(hWnd, (UINT)wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP: OnLButtonUp(hWnd, (UINT)wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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

LRESULT OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hWnd, &ps);

	for (UINT i = 0; i < SHAPE_COUNT; i++)
	{

		if (Shapes[i].ID == ID_SHAPE_RECTANGLE)
		{
			if (Flag & MK_SHIFT)
			{
				UINT absSubX = abs(Shapes[i].rect.right - Shapes[i].rect.left);
				UINT absSubY = abs(Shapes[i].rect.bottom - Shapes[i].rect.top);

				if (absSubX > absSubY)
				{
					if (Shapes[i].rect.bottom > Shapes[i].rect.top)
						Shapes[i].rect.bottom = Shapes[i].rect.top + absSubX;
					else
						Shapes[i].rect.bottom = Shapes[i].rect.top - absSubX;
				}
				else
				{
					if (Shapes[i].rect.right > Shapes[i].rect.left)
						Shapes[i].rect.right = Shapes[i].rect.left + absSubY;
					else
						Shapes[i].rect.right = Shapes[i].rect.left - absSubY;
				}
			}
			::Rectangle(hdc, Shapes[i].rect.left, Shapes[i].rect.top, Shapes[i].rect.right, Shapes[i].rect.bottom);
		}
		else if (Shapes[i].ID == ID_SHAPE_ELLIPSE)
		{
			if (Flag & MK_SHIFT)
			{
				UINT xdiff = abs(Shapes[i].rect.right - Shapes[i].rect.left);
				UINT ydiff = abs(Shapes[i].rect.bottom - Shapes[i].rect.top);

				if (xdiff > ydiff)
				{
					if (Shapes[i].rect.bottom > Shapes[i].rect.top)
						Shapes[i].rect.bottom = Shapes[i].rect.top + xdiff;
					else
						Shapes[i].rect.bottom = Shapes[i].rect.top - xdiff;
				}
				else
				{
					if (Shapes[i].rect.right > Shapes[i].rect.left)
						Shapes[i].rect.right = Shapes[i].rect.left + ydiff;
					else
						Shapes[i].rect.right = Shapes[i].rect.left - ydiff;
				}
			}
			::Ellipse(hdc, Shapes[i].rect.left, Shapes[i].rect.top, Shapes[i].rect.right, Shapes[i].rect.bottom);
		}
		else if (Shapes[i].ID == ID_SHAPE_LINE) {//ID_SHAPE_LINE
			::MoveToEx(hdc, Shapes[i].rect.left, Shapes[i].rect.top, NULL);
			::LineTo(hdc, Shapes[i].rect.right, Shapes[i].rect.bottom);
		}



	}

	/*Ellipse(hdc, 200, 100, 100, 50);
	Rectangle(hdc, 500, 200, 300, 400);*/
	// TODO: Add any drawing code that uses hdc here...
	::EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnMouseMove(HWND hWnd, UINT nCtrl, UINT x, UINT y)
{
	if (!isRubberBand)
	{
		return 0;
	}
	Flag = nCtrl;
	Draw(hWnd);
	currentPoint.x = x;
	currentPoint.y = y;
	Draw(hWnd);
	return 0;
}

LRESULT OnLButtonDown(HWND hWnd, UINT nCtrl, UINT x, UINT y)
{
	startPoint.x = x;
	startPoint.y = y;

	currentPoint.x = x;
	currentPoint.y = y;

	isRubberBand = true;
	Flag = nCtrl;
	Draw(hWnd);
	return 0;
}

LRESULT OnLButtonUp(HWND hWnd, UINT nCtrl, UINT X, UINT y)
{
	if (!isRubberBand) return 0;
	Flag = nCtrl;
	Draw(hWnd);
	if (::GetMenuState(hMenu, ID_SHAPE_RECTANGLE, MF_BYCOMMAND) & MF_CHECKED)
	{
		Shapes[currentIndex].ID = ID_SHAPE_RECTANGLE;
	}
	else if (::GetMenuState(hMenu, ID_SHAPE_ELLIPSE, MF_BYCOMMAND) & MF_CHECKED)
	{
		Shapes[currentIndex].ID = ID_SHAPE_ELLIPSE;
	}
	else
	{
		Shapes[currentIndex].ID = ID_SHAPE_LINE;
	}
	//Shapes[currentIndex].ID = ID_SHAPE_ELLIPSE;
	Shapes[currentIndex].rect.left = startPoint.x;
	Shapes[currentIndex].rect.top = startPoint.y;
	Shapes[currentIndex].rect.right = currentPoint.x;
	Shapes[currentIndex].rect.bottom = currentPoint.y;
	currentIndex = (++currentIndex) % SHAPE_COUNT;
	isRubberBand = false;
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
	return 0;
}

void Draw(HWND hWnd)
{
	POINT tempStartPoint;
	POINT tempCurrentPoint;
	tempStartPoint.x = min(startPoint.x, currentPoint.x);
	tempStartPoint.y = min(startPoint.y, currentPoint.y);
	tempCurrentPoint.x = max(startPoint.x, currentPoint.y);
	tempCurrentPoint.y = max(startPoint.y, currentPoint.y);

	HDC hdc;
	hdc = ::GetDC(hWnd);
	::SetROP2(hdc, R2_NOT);
	::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));

	if (::GetMenuState(hMenu, ID_SHAPE_RECTANGLE, MF_BYCOMMAND) & MF_CHECKED)
	{
		if (Flag & MK_SHIFT)
		{
			long absSubx = abs(currentPoint.x - startPoint.x);
			long absSuby = abs(currentPoint.y - startPoint.y);

			if (absSubx > absSuby)
			{
				if (currentPoint.y > startPoint.y)
				{
					tempCurrentPoint.y = currentPoint.y + absSubx;
				}
				else
				{
					tempCurrentPoint.y = currentPoint.y - absSubx;
				}
			}
			else
			{
				if (currentPoint.x > startPoint.x)
				{
					tempCurrentPoint.x = startPoint.x + absSuby;
				}
				else
				{
					tempCurrentPoint.x = startPoint.x - absSuby;
				}
			}
			::Rectangle(hdc, startPoint.x, startPoint.y, tempCurrentPoint.x, tempCurrentPoint.y);
		}
		else
			::Rectangle(hdc, startPoint.x, startPoint.y, currentPoint.x, currentPoint.y);
	}
	else if (::GetMenuState(hMenu, ID_SHAPE_ELLIPSE, MF_BYCOMMAND) & MF_CHECKED)
	{
		if (Flag & MK_SHIFT)
		{
			long absSubx = abs(currentPoint.x - startPoint.x);
			long absSuby = abs(currentPoint.y - startPoint.y);

			if (absSubx > absSuby)
			{
				if (currentPoint.y > startPoint.y)
				{
					tempCurrentPoint.y = currentPoint.y + absSubx;
				}
				else
				{
					tempCurrentPoint.y = currentPoint.y - absSubx;
				}
			}
			else
			{
				if (currentPoint.x > startPoint.x)
				{
					tempCurrentPoint.x = startPoint.x + absSuby;
				}
				else
				{
					tempCurrentPoint.x = startPoint.x - absSuby;
				}
			}
			::Ellipse(hdc, tempStartPoint.x, tempStartPoint.y, tempCurrentPoint.x, tempCurrentPoint.y);
		}
		else
			::Ellipse(hdc, startPoint.x, startPoint.y, currentPoint.x, currentPoint.y);
	}
	else if (::GetMenuState(hMenu, ID_SHAPE_LINE, MF_BYCOMMAND) & MF_CHECKED)
	{
		::MoveToEx(hdc, startPoint.x, startPoint.y, NULL);
		::LineTo(hdc, currentPoint.x, currentPoint.y);
	}
	else {}

	::ReleaseDC(hWnd, hdc);
}
