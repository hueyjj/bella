# include <stdio.h>
# include <stdlib.h>
# include <getopt.h>
# include <string.h>
# include <unistd.h>
# include <time.h>

# include <Windows.h>
# include "bella.h"

static void usage(void);
static void print_last_error(void);

static void usage(void)
{
    printf("Usage: ./bella.exe [OPTIONS]\n\n");
    printf("Default save directory: ./saves/\n");
    printf("Default bindings:\n"
           "    hotkey1     <control 1>    Download media from highlighted url.\n"
           "    hotkey2     <control 2>    Download media from url in clipboard.\n"
           "    hotkey3     <control 3>    Download all in ./bellaslist.txt\n"
           "    hotkey4     <control 4>    Store highlighted content to ./bellaslist.txt\n"
           "    hotkey5     <control `>    Exit bella\n"
           "    hotkey6     <control 0>    Exit bella\n"
           );

    printf("Options:\n"
           "    -h, --help              Prints this help text and exit.\n"
           );
}

static void print_last_error(void)
{
    DWORD errorMessageID = GetLastError();
    if (errorMessageID != 0)
    {
        LPSTR messageBuffer = NULL;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                (LPSTR)&messageBuffer, 0, NULL);
        
        printf("Last error: %s\n", messageBuffer);

        LocalFree(messageBuffer);
    }
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    opterr = 0;

    const struct option long_options[] = {
        {"help", no_argument, 0, 'h'}
    };

    int option, option_index = 0;
    while ( (option = getopt_long(argc, argv, "h", 
                                long_options, &option_index)) != -1) 
    {
        switch (option)
        {
            case 'h':
                usage();
                exit( EXIT_SUCCESS );
                break;

            default:
                abort();
        }
    }

    printf( "Checking if processor is available..." );
    if ( system(NULL) ) puts ("Ok");
    else                
    { 
        fprintf( stderr, "Unable to spawn subprocess via shell\n" );
        exit (EXIT_FAILURE); 
    }

    // Register hot keys
    reghotkeys();

    int *key;
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            switch (msg.wParam)
            {
                case (VK_HOTKEY1):
                    key = (int *) hotkeys[VK_HOTKEY1].keys; 

                    while ( ishotkeydown(key) ) { nanosleep(&mssleep, NULL); }

                    //printf("LAlt: %d RAlt: %d Alt: %d Ctrl: %d one: %d\n",
                    //        GetAsyncKeyState(VK_LMENU),
                    //        GetAsyncKeyState(VK_RMENU),
                    //        GetAsyncKeyState(VK_MENU),
                    //        GetAsyncKeyState(VK_LCONTROL),
                    //        GetAsyncKeyState(VK_1));

                    download_hl();

                    //printf("Number of events: %d\n", result);

                    break;

                case (VK_HOTKEY2):
                    break;

                case (VK_HOTKEY3):
                    break;

                case (VK_HOTKEY4):
                    break;

                case (VK_HOTKEY5):
                    printf( "Goodbye! Bella is leaving you.\n" );
                    exit( EXIT_SUCCESS );
                    break;

                case (VK_HOTKEY6):
                    printf( "Sayonora! Bella will see you on the other side.\n" );
                    exit( EXIT_SUCCESS );
                    break;

                default:
                    printf( "Something has gone horribly wrong\n" );
                    abort();
            }

            // remove duplicate hotkey inputs
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)); 
        }
    }

    return 0;
}
