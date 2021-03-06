// Win32App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32App.h"

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

HWND hMessage;
HWND hYellow;
HWND hBlue;
HWND hGreen;

ref class Globals
{
public:
	static WpfControl::MyControl ^wpfControl;
};

[System::STAThreadAttribute] // Needs to be an STA thread to play nicely with WPF
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
    LoadStringW(hInstance, IDC_WIN32APP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32APP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32APP);
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
      300, 300, 900, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void OnMessageReceived(System::Object ^sender, System::String ^message)
{
	SetWindowTextW(hMessage, (LPCWSTR)System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(message).ToPointer());
}

// Attach a WPF control to the Win32 window
HWND AttachWpfControl(HWND parent, int x, int y, int width, int height)
{
	System::Windows::Interop::HwndSourceParameters^ sourceParams = gcnew System::Windows::Interop::HwndSourceParameters("Wpf" /*any name*/);

	sourceParams->PositionX = x;
	sourceParams->PositionY = y;
	sourceParams->Height = height;
	sourceParams->Width = width;

	sourceParams->ParentWindow = System::IntPtr::IntPtr(parent);
	sourceParams->WindowStyle = WS_VISIBLE | WS_CHILD; // style

	System::Windows::Interop::HwndSource^ source = gcnew System::Windows::Interop::HwndSource(*sourceParams);
	Globals::wpfControl = gcnew WpfControl::MyControl();
	Globals::wpfControl->MessageReceived += gcnew WpfControl::MyControl::MessageHandler(OnMessageReceived);

	source->RootVisual = Globals::wpfControl;
	return (HWND)source->Handle.ToPointer();
}

void CreateWin32Controls(HWND parent, int x, int y, int width, int height)
{
	/*************** GroupBox ****************/
	CreateWindow("Button", "Win32", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, x, y, width, height, parent, NULL, NULL, NULL);

	/*************** Message Controls ****************/
	CreateWindow("Static", "Received Message: ", WS_CHILD | WS_VISIBLE, x + 10, y + 30, 155, 20, parent, NULL, NULL, NULL);
	hMessage = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED, x + 10, y + 55, width - 20, 20, parent, NULL, NULL, NULL);

	/*************** Background Controls ****************/
	CreateWindow("Static", "Change WPF's Background", WS_CHILD | WS_VISIBLE, x + 10, y + 105, 200, 20, parent, NULL, NULL, NULL);

	// The `hWndParent` must be the handle of the Window so that the `WM_COMMAND` would be fired
	hYellow = CreateWindowEx(0, "Button", "LightYellow", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, x + 10, y + 130, width - 20, 20, parent, (HMENU)IDB_RADIO_YELLOW, NULL, NULL);
	hBlue = CreateWindowEx(0, "Button", "LightCyan", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, x + 10, y + 150, width - 20, 20, parent, (HMENU)IDB_RADIO_Cyan, NULL, NULL);
	hGreen = CreateWindowEx(0, "Button", "LightGray", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, x + 10, y + 170, width - 20, 20, parent, (HMENU)IDB_RADIO_Gray, NULL, NULL);
	
	// Default to check the first RadioButton
	SendMessage(hYellow, BM_SETCHECK, BST_CHECKED, 1);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDB_RADIO_YELLOW:
				if (IsDlgButtonChecked(hWnd, IDB_RADIO_YELLOW) == BST_CHECKED)
				{
					Globals::wpfControl->Background = gcnew System::Windows::Media::SolidColorBrush(System::Windows::Media::Colors::LightYellow);
				}
				break;
			case IDB_RADIO_Cyan:
				if (IsDlgButtonChecked(hWnd, IDB_RADIO_Cyan) == BST_CHECKED)
				{
					Globals::wpfControl->Background = gcnew System::Windows::Media::SolidColorBrush(System::Windows::Media::Colors::LightCyan);
				}
				break;
			case IDB_RADIO_Gray:
				if (IsDlgButtonChecked(hWnd, IDB_RADIO_Gray) == BST_CHECKED)
				{
					Globals::wpfControl->Background = gcnew System::Windows::Media::SolidColorBrush(System::Windows::Media::Colors::LightGray);
				}
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
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_CREATE:
		AttachWpfControl(hWnd, 10, 10, 500, 350);
		CreateWin32Controls(hWnd, 550, 10, 300, 350);
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
