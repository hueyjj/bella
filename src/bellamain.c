# include <stdio.h>
# include <stdlib.h>
# include <getopt.h>
# include <string.h>
# include <unistd.h>
# include <time.h>
# include <Windows.h>

# include "bella.h"

static void usage(void);

static void usage(void)
{
    printf("Usage: ./bella.exe [OPTIONS]\n\n");
    printf("Default save directory: ./saves/ ./bellascandy/\n");
    printf("Default save file: ./bellaslist.txt\n");
    printf("Default bindings:\n"
           "    hotkey1     <control 1>    Download media from highlighted url.\n"
           "    hotkey2     <control 2>    Download media from url in clipboard.\n"
           "    hotkey3     <control 3>    Download all in ./bellaslist.txt\n"
           "    hotkey4     <control 4>    Store highlighted content to ./bellaslist.txt\n"
           "    hotkey5     <control 5>    Store clipboard content to ./bellaslist.txt\n"
           "    hotkey6     <control `>    Exit bella\n"
           "    hotkey7     <control F1>   Exit bella\n"
           "    hotkey8     <control 9>    Check if bella is running\n"
           );

    printf("Options:\n"
           "    -h, --help              Prints this help text and exit.\n"
           );
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        usage();
        exit(EXIT_SUCCESS);
    }

    pDEBUG("Checking if processor is available...");
    if (system(NULL)) pDEBUG("Ok\n");
    else                
    { 
        pERROR("Unable to spawn subprocess via shell\n");
        exit(EXIT_FAILURE); 
    }

    // Register hot keys
    if (!reghotkeys()) 
    {
        error("unable to register a couple primary keys");
        exit(EXIT_SUCCESS);
    }

    int *key;
    MSG msg = {0};
    while (GetMessage(&msg, NULL, WM_HOTKEY, WM_HOTKEY) > 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            key = (int *) hotkeys[msg.wParam].keys; 
            while (ishotkeydown(key)) { nanosleep(&mssleep, NULL); }

            switch (msg.wParam)
            {
                case (VK_HOTKEY1):
                    //pDEBUG("LAlt: %d RAlt: %d Alt: %d Ctrl: %d one: %d\n",
                    //        GetAsyncKeyState(VK_LMENU),
                    //        GetAsyncKeyState(VK_RMENU),
                    //        GetAsyncKeyState(VK_MENU),
                    //        GetAsyncKeyState(VK_LCONTROL),
                    //        GetAsyncKeyState(VK_1));
                    
                    download_hl();
                    store_clipboard();
                    //pDEBUG("Number of events: %d\n", result);
                    break;

                case (VK_HOTKEY2):
                    download_clipboard();
                    store_clipboard();
                    break;

                case (VK_HOTKEY3):
                    download_bellaslist();
                    break;

                case (VK_HOTKEY4):
                    store_hl();
                    break;

                case (VK_HOTKEY5):
                    store_clipboard();
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

                default:
                    pDEBUG("Thanks a lot for breaking this program.\n");
                    abort();
            }

            // remove duplicate hotkey inputs
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)); 
        }
    }

    return 0;
}
