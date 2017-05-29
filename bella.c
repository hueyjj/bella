# define _GNU_SOURCE 

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

# include "bella.h"

static void copy(void);
static char *getclipboardS(void);
static void download(const char *url);

static const char savedir[] = "./saves";
static const short mostsigbit = (1 << 15);

static void copy(void)
{
    UINT result;
    const int maxretry = 10;
    int retry = 0;
    char *prev, *curr;

    puts("prev"); prev = getclipboardS();

    do 
    {
        retry++;
        result = SendInput(4, (LPINPUT) copykey, sizeof(INPUT));
        for (int i = 0; i < 5; ++i) nanosleep(&mssleep, NULL); // 500 ms
        puts("curr");
    } while (curr = getclipboardS(), strcmp(prev, curr) == 0 && retry < maxretry);
}

/* Retrieves text from clipboard */
static char *getclipboardS(void)
{
    HWND cbOwner = GetClipboardOwner();
    if (!OpenClipboard(cbOwner)) 
    {
        fprintf(stderr, "Failed to open clipboard\n");
    }
        
    HANDLE clipboard;
    clipboard = GetClipboardData(CF_TEXT);

    if (!clipboard) 
    { 
        fprintf(stderr, "Clipboard handle error\n"); 
        return (char *) 0;
    }

    LPSTR text;
    text = GlobalLock(clipboard);
    if (!text)
    {
        fprintf(stderr, "Unable to read text from clipboard\n");
        return (char *) 0;
    }
    printf("\nIn clipboard: %s\n", text); 

    GlobalUnlock(clipboard);
    CloseClipboard();

    return text;
}

static void download(const char *url)
{

    struct stat sb = {0};
    if ( !(stat( savedir, &sb) == 0 && S_ISDIR(sb.st_mode)) )
        mkdir( savedir );

    char *cmd;
    const char *youtubedl = "youtube-dl  -o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata";
    
    asprintf(&cmd, "(cd %s && %s %s)", savedir, youtubedl, url);

    printf("\nExecuting... %s\n\n", cmd); 

    FILE *s = popen(cmd, "r"); 

    // Check sub process output
    char buffer[1024];
    while (fgets(buffer, 1024, s) != NULL)
        printf("%s", buffer);

    pclose(s);
}

int ishotkeydown(int hotkey[])
{
    int flag = 0;
    for (int i = 0; i < size(hotkey); ++i)
    {
        if ( (GetAsyncKeyState(hotkey[i]) & mostsigbit) == mostsigbit)
            flag = 1; // Key is down
    }
    return (flag == 1) ? true : false;
}

void download_hl(void)
{
    copy();
    char *url = getclipboardS();
    download(url);
}

int reghotkeys(void)
{
    int exitflag = 0;

    if (!RegisterHotKey(NULL, VK_HOTKEY1, MOD_CONTROL, VK_1))
    {
        fprintf(stderr, "Failed to register hotkey 1\n");
        exitflag = 1;
    }
    
    if (!RegisterHotKey(NULL, VK_HOTKEY2, MOD_CONTROL, VK_2))
    {
        fprintf(stderr, "Failed to register hotkey 2\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY3, MOD_CONTROL, VK_3))
    {
        fprintf(stderr, "Failed to register hotkey 3\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY4, MOD_CONTROL, VK_4))
    {
        fprintf(stderr, "Failed to register hotkey 4\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY5, MOD_CONTROL, VK_OEM_3)) // Control + `
    {
        fprintf(stderr, "Failed to register hotkey 5\n");
        exitflag = 1;
    }

    if ( !RegisterHotKey(NULL, VK_HOTKEY6, MOD_CONTROL, VK_0) )
    {
        fprintf(stderr, "Failed to register hotkey 6\n");
        exitflag = 1;
    }
    return exitflag;
}
