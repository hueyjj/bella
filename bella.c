// TODO add linux compatibility
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <time.h>

# include <Windows.h>
# include "bella.h"

void usage(void);
void print_last_error(void);

void usage(void)
{
    printf("Usage: ./bella [OPTIONS]\n\n");
    printf("Default bindings:\n"
           "    <ctrl alt a>    Download media from clipboard url.\n"
           "    <ctrl alt s>    Append clipboard content to ./bellaslist.txt\n");
    printf("Options:\n"
           "    -h, --help              Prints this help text and exit.\n"
           "    -l, --load-config=FILE  Load hotkeys from config file.\n"
           "        --bin=DIRECTORY     Sets youtube-dl bin location and stores in config file.\n"
           "        --args=ARGS         Sets youtube-dl arguments and stores in config file\n"
           "        --save=DIRECTORY    Directory to store downloads.\n");
                
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    
    //usage();
    //exit(0);

    // TODO set bindings for hotkeys in user config; include default bindings
    if (RegisterHotKey(NULL, VK_HOTKEY1, MOD_CONTROL | MOD_ALT, VK_D))
    {
        printf("Hotkey registered [1].\n");
    }
    else
    {
        fprintf(stderr, "Failed to register hotkey\n");
        exit( EXIT_FAILURE );
    }

    if (RegisterHotKey(NULL, VK_HOTKEY2, MOD_ALT | MOD_CONTROL, VK_B))
    {
        printf("Hotkey registered [2].\n");
    }
    else
    {
        fprintf(stderr, "Failed to register hotkey\n");
        exit( EXIT_FAILURE );
    }

    INPUT copy[4];
    ZeroMemory(copy, sizeof(copy));

    copy[0].type = INPUT_KEYBOARD;
    copy[0].ki.wVk = VK_LCONTROL;
    copy[0].ki.wScan = MapVirtualKey(VK_LCONTROL, MAPVK_VK_TO_VSC);
    copy[0].ki.dwFlags = KEYEVENTF_SCANCODE;

    copy[1].type = INPUT_KEYBOARD;
    copy[1].ki.wVk = VK_C;
    copy[1].ki.wScan = MapVirtualKey(VK_C, MAPVK_VK_TO_VSC);
    copy[1].ki.dwFlags = KEYEVENTF_SCANCODE;

    copy[2].type = INPUT_KEYBOARD;
    copy[2].ki.wVk = VK_C;
    copy[2].ki.wScan = copy[1].ki.wScan;
    copy[2].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;

    copy[3].type = INPUT_KEYBOARD;
    copy[3].ki.wVk = VK_LCONTROL;
    copy[3].ki.wScan = copy[0].ki.wScan;
    copy[3].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        printf("message: %d wParam %d lParam %d time %d\n",
                msg.message, msg.wParam, msg.lParam, msg.time);
        if (msg.message == WM_HOTKEY)
        {
            switch(msg.wParam)
            {
                case(VK_HOTKEY1):
                    printf("VK_HOTKEY1 entered\n");

                    while(GetAsyncKeyState(VK_D)        < 0 ||
                          GetAsyncKeyState(VK_LCONTROL) < 0 ||
                          GetAsyncKeyState(VK_MENU)     < 0)
                    {
                        printf("d is pressed\n");
                        nanosleep(&mssleep, NULL);
                    }

                    printf("Left control: %d d: %d c: %d alt: %d\n",
                            GetAsyncKeyState(VK_LCONTROL),
                            GetAsyncKeyState(VK_D),
                            GetAsyncKeyState(VK_C),
                            GetAsyncKeyState(VK_MENU));

                    UINT result = SendInput(4, copy, sizeof(INPUT));

                    // remove duplicate hotkey inputs
                    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)); 

                    printf("Number of events: %d\n", result);

                    printf("Left control: %d d: %d c: %d alt: %d\n",
                            GetAsyncKeyState(VK_LCONTROL),
                            GetAsyncKeyState(VK_D),
                            GetAsyncKeyState(VK_C),
                            GetAsyncKeyState(VK_MENU));

                    if (!result)
                        print_last_error();

                    break;
                case(VK_HOTKEY2):
                    printf("VK_HOTKEY2 entered\n");
                    break;
                default:
                    printf("Something has gone horribly wrong\n");
            }
        }

    }

    HWND cbOwner = GetClipboardOwner();
    if (OpenClipboard(cbOwner)) 
    {
        printf("Opened clipboard\n");
    }
    else
    {
        fprintf(stderr, "Failed to open clipboard\n");
    }
        
    HANDLE clipboard;
    clipboard = GetClipboardData(CF_TEXT);

    if (!clipboard) 
    { 
        fprintf(stderr, "clipboard error\n"); 
        print_last_error(); 
        exit( EXIT_FAILURE ); 
    }
    else 
    { 
        LPSTR text;
        text = GlobalLock(clipboard);
        if (!text)
        {
            fprintf(stderr, "invalid text?\n");
            exit( EXIT_FAILURE);
        }
        else
        {
            printf("in clipboard: %s\n", text); 
        }

        GlobalUnlock(clipboard);
    }
    
    CloseClipboard();

    //int i;
    //printf ("Checking if processor is available...");
    //if (system(NULL)) puts ("Ok");
    //else exit (EXIT_FAILURE);
    //printf ("Executing system command...\n");
    //FILE *s = popen("youtube-dl  -o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata https://www.youtube.com/watch?v=UXGJCzGx8w4", "r"); 

    //char buffer[256];
    //while (fgets(buffer, 256, s) != NULL)
    //{
    //    //printf("buffer: %s\n", buffer);
    //}

    ////i= system ("youtube-dl  -o \"%(title)s.%(ext)s\" -i --format m4a \
    //        --embed-thumbnail --add-metadata https://www.youtube.com/watch?v=UXGJCzGx8w4");
    //printf ("The value returned was: %d.\n",i);
    //pclose(s);
    return 0;
}

void print_last_error(void)
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
        
        //char message[size];
        //message = strcpy(message, messageBuffer);

        printf("error: %s\n", messageBuffer);
        //Free the buffer.
        LocalFree(messageBuffer);
    }
}
