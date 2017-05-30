# define _GNU_SOURCE 

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

# include "bella.h"

# define MAX_SZ 2048 

static int copy(void);
static char *getclipboardS(void);
static void download(const char *url);

static const char savedir[] = "./saves";
static const short mostsigbit = (1 << 15);

static int copy(void)
{
    UINT result;
    const int maxretry = 20;
    int retry = 0;
    char prev[MAX_SZ], curr[MAX_SZ];

    //prev = getclipboardS();
    strcpy(prev, getclipboardS());

    do 
    {
        pDEBUG("prev: %s\n", prev);
        pDEBUG("curr: %s\n", curr);
        retry++;
        result = SendInput(4, (LPINPUT) copykey, sizeof(INPUT));
        for (int i = 0; i < 5; ++i) nanosleep(&mssleep, NULL); // 500 ms
    } while (strcpy(curr, getclipboardS()), strcmp(prev, curr) == 0 && retry < maxretry);

    return (retry < maxretry) ? true : false;
}

/* Retrieves text from clipboard */
static char *getclipboardS(void)
{
    HWND cbOwner = GetClipboardOwner();
    if (!OpenClipboard(cbOwner)) 
    {
        pERROR("Failed to open clipboard\n");
    }
        
    HANDLE clipboard;
    clipboard = GetClipboardData(CF_TEXT);

    if (!clipboard) 
    { 
        pERROR("Clipboard handle error\n"); 
        return (char *) 0;
    }

    LPSTR text;
    text = GlobalLock(clipboard);
    if (!text)
    {
        pERROR("Unable to read text from clipboard\n");
        return (char *) 0;
    }

    GlobalUnlock(clipboard);
    CloseClipboard();

    return text;
}

static void download(const char *url)
{
    struct stat sb = {0};
    if (stat(savedir, &sb) != 0 || !S_ISDIR(sb.st_mode))
        mkdir(savedir);

    //char *cmd;
    const char *youtube = "youtube-dl";
    const char *param = "-o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata";
    char *paramb;

    asprintf(&paramb, "%s %s", param, url);

    //const char *youtubedl = "youtube-dl -o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata";
    //asprintf(&cmd, "(cd %s && %s %s)", savedir, youtubedl, url);
    //pDEBUG("\nExecuting...\n%s\n\n", cmd); 


    pDEBUG("\nExecuting..\n\n");
    int sh = (int) ShellExecute(NULL, "open", youtube, paramb, savedir, SW_HIDE);
    pDEBUG("shell execute code: %d ( > 32 == success)\n", sh);

    //FILE *s = popen(cmd, "r"); 

    // Check sub process output
    //char buffer[2048];
    //while (fgets(buffer, 1024, s) != NULL);
    //    pDEBUG("%s", buffer);

    //pclose(s);
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
    if (copy() == false)
    {
        pERROR("Failed to copy\n");
        return;
    }

    // TODO add a way to verify if downloaded and if it's the right file

    char *url = getclipboardS();
    pDEBUG("In clipboard: %s\n", url);
    download(url);
}

void download_clipboard(void)
{
    char *url = getclipboardS();
    download(url);
}

int reghotkeys(void)
{
    int exitflag = 0;

    if (!RegisterHotKey(NULL, VK_HOTKEY1, MOD_CONTROL, VK_1))
    {
        pERROR("Failed to register hotkey 1\n");
        exitflag = 1;
    }
    
    if (!RegisterHotKey(NULL, VK_HOTKEY2, MOD_CONTROL, VK_2))
    {
        pERROR("Failed to register hotkey 2\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY3, MOD_CONTROL, VK_3))
    {
        pERROR("Failed to register hotkey 3\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY4, MOD_CONTROL, VK_4))
    {
        pERROR("Failed to register hotkey 4\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY5, MOD_CONTROL, VK_OEM_3)) // Control + `
    {
        pERROR("Failed to register hotkey 5\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY6, MOD_CONTROL, VK_0))
    {
        pERROR("Failed to register hotkey 6\n");
        exitflag = 1;
    }
    return exitflag;
}
