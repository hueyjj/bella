// TODO add linux compatibility
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <Windows.h>
# include "bella.h"

void print_manual(void);

void print_manual(void)
{
    printf("Usage: ./bella [OPTIONS]\n\n");
    printf("Default bindings:\n"
           "    <ctrl alt a>    Download media from clipboard url.\n"
           "    <ctrl alt s>    Append clipboard content to ./bellaslist.txt\n");
    printf("Options:\n"
           "    -h, --help              Prints this help text and exit.\n"
           "    -l, --load-config=FILE  Load hotkeys from config file.\n"
           "        --set-bin=DIRECTORY Sets youtube-dl bin location and stores in config file.\n"
           "        --set-args=ARGS     Sets youtube-dl arguments and stores in config file\n"
           "        --save=DIRECTORY    Directory to store downloads.\n");
                
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    
    print_manual();
    exit(0);

    // TODO set bindings for hotkeys in user config; include default bindings
    if (RegisterHotKey(NULL, 0x01, MOD_ALT | MOD_CONTROL, 0x43))
    {
        printf("Hotkey registered.\n");
    }
    else
    {
        fprintf(stderr, "Failed to register hotkey\n");
        exit( EXIT_FAILURE );
    }

    if (RegisterHotKey(NULL, 0x02, MOD_ALT | MOD_CONTROL, 0x4A))
    {
        printf("Hotkey registered.\n");
    }
    else
    {
        fprintf(stderr, "Failed to register hotkey\n");
        exit( EXIT_FAILURE );
    }

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            printf("WM_HOTKEY received\n");
            switch(msg.wParam)
            {
                case(VK_HOTKEY1):
                    printf("VK_HOTKEY1 entered\n");
                    break;
                case(VK_HOTKEY2):
                    printf("VK_HOTKEY2 entered\n");
                    break;
                default:
                    printf("Something has gone horribly wrong\n");
            }
            exit( EXIT_SUCCESS );
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

    int i;
    printf ("Checking if processor is available...");
    if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
    printf ("Executing system command...\n");
    FILE *s = popen("youtube-dl  -o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata https://www.youtube.com/watch?v=UXGJCzGx8w4", "r"); 

    char buffer[256];
    while (fgets(buffer, 256, s) != NULL)
    {
        //printf("buffer: %s\n", buffer);
    }

    //i= system ("youtube-dl  -o \"%(title)s.%(ext)s\" -i --format m4a \
            --embed-thumbnail --add-metadata https://www.youtube.com/watch?v=UXGJCzGx8w4");
    printf ("The value returned was: %d.\n",i);
    pclose(s);
    return 0;
}
