#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <Windows.h>

#include "bella.h"

static TCHAR szWindowClass[] = TEXT("Bella application");  
static TCHAR szTitle[] = TEXT("Bella");  

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  
NOTIFYICONDATA initSystemTray();

//static void usage(void);
//
//static void usage(void)
//{
//    printf("Usage: ./bella.exe [OPTIONS]\n\n");
//    printf("Default save directory: ./saves/ ./bellascandy/\n");
//    printf("Default save file: ./bellaslist.txt\n");
//    printf("Default bindings:\n"
//           "    hotkey1     <control 1>    Download media from highlighted url.\n"
//           "    hotkey2     <control 2>    Download media from url in clipboard.\n"
//           "    hotkey3     <control 3>    Download all in ./bellaslist.txt\n"
//           "    hotkey4     <control 4>    Store highlighted content to ./bellaslist.txt\n"
//           "    hotkey5     <control 5>    Store clipboard content to ./bellaslist.txt\n"
//           "    hotkey6     <control `>    Exit bella\n"
//           "    hotkey7     <control F1>   Exit bella\n"
//           "    hotkey8     <control 9>    Check if bella is running\n"
//           );
//
//    printf("Options:\n"
//           "    -h, --help              Prints this help text and exit.\n"
//           );
//}

NOTIFYICONDATA initSystemTray()
{
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = SYSTEM_TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = (HICON) LoadImage(NULL, TEXT("Happy-face.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
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
        printLastError(NULL);
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
        printLastError(NULL);
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
    while (GetMessage(&msg, NULL, WM_HOTKEY, WM_HOTKEY) > 0)
    {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            // remove duplicate hotkey inputs
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)); 
    }

    exit(EXIT_SUCCESS);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message) 
    {
        case WM_CREATE:
            systemTrayMenu = CreatePopupMenu();
            AppendMenu(systemTrayMenu, MF_STRING, SYSTEM_TRAY_ICON_ID, TEXT("EXIT"));
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &systemTrayIcon);
            break;
        case WM_HOTKEY:
            { 
            int *key = (int *) hotkeys[wParam].keys; 
            while (isHotkeyDown(key)) { nanosleep(&mssleep, NULL); } 
            }
            
            switch (wParam)
            {
                case (VK_HOTKEY1):
                    //pDEBUG("LAlt: %d RAlt: %d Alt: %d Ctrl: %d one: %d\n",
                    //        GetAsyncKeyState(VK_LMENU),
                    //        GetAsyncKeyState(VK_RMENU),
                    //        GetAsyncKeyState(VK_MENU),
                    //        GetAsyncKeyState(VK_LCONTROL),
                    //        GetAsyncKeyState(VK_1));
                    
                    downloadhl();
                    storeClipboard();
                    //pDEBUG("Number of events: %d\n", result);
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
                    exit(EXIT_SUCCESS);
                    break;

                case (VK_HOTKEY7):
                    pDEBUG("\nSayonora! Bella will see you on the other side.\n");
                    error("bella left you\nSee you on the other side");
                    exit(EXIT_SUCCESS);
                    break;

                case (VK_HOTKEY8):
                    pDEBUG("bella is online <3");
                    error("bella is online <3"); // hack to show a message
                    break;
                
                case WM_DESTROY:  
                     PostQuitMessage(0);  
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
                                balloonTip("Right click up");
                            }
                    }
                break;
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);  
}  
