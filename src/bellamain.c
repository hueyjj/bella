#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <Windows.h>

#include "resource.h"
#include "bella.h"

static TCHAR szWindowClass[] = TEXT("Bella application");  
static TCHAR szTitle[] = TEXT("Bella");  

static HINSTANCE hInst;
static NOTIFYICONDATA systemTrayIcon;
static HMENU systemTrayMenu;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  
static NOTIFYICONDATA initSystemTray();

static NOTIFYICONDATA initSystemTray()
{
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = SYSTEM_TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_TRAYICON;
    //nid.hIcon = (HICON) LoadImage(NULL, TEXT("Happy-face.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    nid.hIcon = (HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_BELLA), 
                                  IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
    printLastError();
    strcpy(nid.szTip, TEXT("bella's program"));
    return nid;
}

int WINAPI WinMain(HINSTANCE hInstance,  
                   HINSTANCE hPrevInstance,  
                   LPSTR lpCmdLine,  
                   int nCmdShow)  
{
    pDEBUG("Checking if processor is available...");
    if (!system(NULL)) 
    { 
        pERROR("Unable to spawn subprocess via shell\n");
        exit(EXIT_FAILURE); 
    }
    pDEBUG("ok\n");

    WNDCLASSEX wcex = {0};  

    wcex.cbSize         = sizeof(WNDCLASSEX);  
    wcex.style          = 0;  
    wcex.lpfnWndProc    = WndProc;  
    wcex.cbClsExtra     = 0;  
    wcex.cbWndExtra     = 0;  
    wcex.hInstance      = hInstance;  
    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);  
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszMenuName   = NULL;  
    wcex.lpszClassName  = szWindowClass;  
    wcex.hIconSm        = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));  

    if (!RegisterClassEx(&wcex))  
    {  
        printLastError();
        exit(EXIT_FAILURE);
    } 
   
    hInst = hInstance;

    HWND windowHwnd = CreateWindow(szWindowClass,  
                             szTitle,  
                             WS_OVERLAPPEDWINDOW,  
                             CW_USEDEFAULT, CW_USEDEFAULT,  
                             10, 10,  
                             NULL,  
                             NULL,  
                             hInstance,  
                             NULL);
    if (!windowHwnd)  
    {  
        printLastError();
        error("Unable to register window with the system");
        exit(EXIT_FAILURE);
    } 

    // Register hot keys
    if (!registerHotkeys(windowHwnd)) 
    {
        error("Unable t register a couple primary keys");
        exit(EXIT_SUCCESS);
    }
 
    hwnd = windowHwnd;

    systemTrayIcon = initSystemTray();
    Shell_NotifyIcon(NIM_ADD, &systemTrayIcon);

    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);
    
    MSG msg = {0};
    while (GetMessage(&msg, hwnd, 0, 0) > 0)
    {
            // remove duplicate hotkey inputs
            while (PeekMessage(&msg, hwnd, WM_HOTKEY, WM_HOTKEY, PM_REMOVE)); 
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }

    // Remove system tray icon on exit
    Shell_NotifyIcon(NIM_DELETE, &systemTrayIcon);
    
    exit(EXIT_SUCCESS);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message) 
    {
        case WM_CREATE:
            systemTrayMenu = CreatePopupMenu();
            AppendMenu(systemTrayMenu, MF_STRING, SYSTEM_TRAY_MENU_EXIT, TEXT("EXIT"));
            break;
        
        case WM_HOTKEY:
            { 
            int *key = (int *) hotkeys[wParam].keys; 
            while (isHotkeyDown(key)) { nanosleep(&mssleep, NULL); } 
            }
            
            switch (wParam)
            {
                case (VK_HOTKEY1):
                    downloadhl();
                    storeClipboard();
                    break;

                case (VK_HOTKEY2):
                    downloadClipboard();
                    storeClipboard();
                    break;

                case (VK_HOTKEY3):
                    downloadBellaslist();
                    break;

                case (VK_HOTKEY4):
                    storeHl();
                    break;

                case (VK_HOTKEY5):
                    storeClipboard();
                    break;

                case (VK_HOTKEY6):
                    pDEBUG("\nGoodbye! Bella is leaving you.\n");
                    error("bella left you");
                    PostQuitMessage(0);
                    break;

                case (VK_HOTKEY7):
                    pDEBUG("\nSayonora! Bella will see you on the other side.\n");
                    error("bella left you\nSee you on the other side");
                    PostQuitMessage(0);
                    break;

                case (VK_HOTKEY8):
                    pDEBUG("bella is online <3");
                    balloonTip("bella is online <3");
                    break;
            }
            break;
        
        case WM_TRAYICON:
            switch (wParam)
            {
                case SYSTEM_TRAY_ICON_ID:
                    switch (lParam)
                    {
                        case WM_RBUTTONUP:
                        {
                            POINT cursorPos;
                            GetCursorPos(&cursorPos);
                            
                            SetForegroundWindow(hwnd);

                            UINT id = TrackPopupMenu(systemTrayMenu, TPM_RETURNCMD | TPM_NONOTIFY,
                                                          cursorPos.x, cursorPos.y, 0, hWnd, NULL);
                            if (id == SYSTEM_TRAY_MENU_EXIT)
                            {
                                PostQuitMessage(0);
                            }
                        }
                    }
                break;
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);  
}  
