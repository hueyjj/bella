#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bella.h"

#define MAX_SZ 2048 

static int  copy(void);
static char *getClipboardString(void);
static void download(const char *, const char *);
static void store(const char *);

static const char savedir[] = "./current_saves";
static const char savedir2[] = "./bellaslist_all";
static const char savefile[] = "./bellaslist.txt";
static const short mostsigbit = (1 << 15);

static const char *youtube = "C:\\ffmpeg\\bin\\youtube-dl"; //TODO Find youtube-dl path using win api
static const char *param = "-o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata";

/* Copy to clipboard only if it's different than the previous content */
static int copy(void)
{
    UINT result;
    const int maxretry = 5;
    int retry = 0;
    char prev[MAX_SZ], curr[MAX_SZ];

    //prev = getClipboardString();
    strcpy(prev, getClipboardString());

    do 
    {
        //pDEBUG("prev: %s\n", prev);
        //pDEBUG("curr: %s\n", curr);
        retry++;
        result = SendInput(4, (LPINPUT) copykey, sizeof(INPUT));
        for (int i = 0; i < 5; ++i) nanosleep(&mssleep, NULL); // 500 ms
    } while (strcpy(curr, getClipboardString()), strcmp(prev, curr) == 0 && retry < maxretry);

    if (retry >= maxretry) error("Unable to copy after max retry");

    return (retry < maxretry) ? true : false;
}

/* Retrieves text from clipboard */
static char *getClipboardString(void)
{
    HWND cbOwner = GetClipboardOwner();
    if (!OpenClipboard(cbOwner)) 
    {
        pERROR("Failed to open clipboard\n");
        error("Failed to open clipboard\n");
    }
        
    HANDLE clipboard;
    clipboard = GetClipboardData(CF_TEXT);

    if (!clipboard) 
    { 
        pERROR("Clipboard handle error\n"); 
        error("Clipboard handle error\n"); 
        return (char *) 0;
    }

    LPSTR text;
    text = GlobalLock(clipboard);
    if (!text)
    {
        pERROR("Unable to read text from clipboard\n");
        error("Unable to read text from clipboard\n");
        return (char *) 0;
    }

    GlobalUnlock(clipboard);
    CloseClipboard();

    return text;
}

static void download(const char *dir, const char *url)
{
    struct stat sb = {0};
    if (stat(dir, &sb) != 0 || !S_ISDIR(sb.st_mode))
        mkdir(dir);

    char *cmd;

    //asprintf(&cmd, "%s %s %s", youtube, param, url);
    asprintf(&cmd, "%s %s %s", youtube, param, url);

    pDEBUG("\nExecuting...\n\n");
    
    STARTUPINFO info;
    PROCESS_INFORMATION processInfo;
    
    ZeroMemory(&info, sizeof(info));
    info.cb = sizeof(info);
    
    ZeroMemory(&processInfo, sizeof(processInfo));
    
    if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, 
                      NULL, dir, &info, &processInfo))
    {
        char *msg;
        asprintf(&msg, "Downloading %s", url);
        balloonTip(msg);
        
        int signalState = WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

        asprintf(&msg, "Download complete for %s", url);
        if (signalState == 0)
            balloonTip(msg);
    }
    //int sh = ShellExecute(NULL, "open", youtube, cmd, dir, SW_HIDE);
    //if (sh <= 32)
    //{
    //    error("download: Unable to execute youtube-dl"); 
    //    return;
    //}
    //pDEBUG("shell execute code: %d ( > 32 == success)\n", sh);
}

static void store(const char *buf)
{
    FILE *outfile;
    outfile = fopen(savefile, "a");

    if (outfile == (FILE *) 0)
    {
        error("store: Unable to open file.");
        exit(EXIT_FAILURE);
    }

    pDEBUG("In buffer: %s\n", buf);
    fputs(buf, outfile);
    fputc('\n', outfile);

    fclose(outfile);
}

/*------------------------------------------------------------------------------------*/

int isHotkeyDown(int hotkey[])
{
    int flag = 0;
    for (int i = 0; i < size(hotkey); ++i)
    {
        if ( (GetAsyncKeyState(hotkey[i]) & mostsigbit) == mostsigbit)
            flag = 1; // Key is down
    }
    return (flag == 1) ? true : false;
}

void downloadhl(void)
{
    copy();

    // TODO add a way to verify if downloaded and if it's the right file

    char *url = getClipboardString();
    pDEBUG("In clipboard: %s\n", url);
    download(savedir, url);
}

void downloadClipboard(void)
{
    char *url = getClipboardString();
    download(savedir, url);
}

void downloadBellaslist(void)
{
    FILE *infile;

    if ( (infile = fopen(savefile, "r")) == (FILE *) 0)
    {
        pERROR("downloadBellaslist: Unable to open bellaslist\n");
        error("downloadBellaslist: Unable to open bellaslist");
        fclose(infile);
        return;
    }

    int dlcount = 0;
    char buffer[2048];
    while ( fgets(buffer, sizeof(buffer), infile) != NULL)
    {
        // no easy to way to check if child shell process is done...
        if (++dlcount % 5 == 0) { sleep(10); }
        download(savedir2, buffer);
    }

    fclose(infile);
}

void storeHl(void)
{
    copy();

    char buffer[MAX_SZ];
    strcpy(buffer, getClipboardString());

    store(buffer);
}

void storeClipboard(void)
{
    store(getClipboardString()); 
}

int registerHotkeys(HWND hwnd)
{
    int errflag = 1; // false
    for (int id = VK_HOTKEY1; id <= VK_HOTKEY8; ++id)
    {
        if (!RegisterHotKey(hwnd, id, MOD_CONTROL, hotkeys[id].keys[1]))
        {
            char helpermsg[2056];
            sprintf(helpermsg, "[hotkey %d (decimal) check msdn virtual key code]", 
                    hotkeys[id].keys[1]);
            printLastError(helpermsg);
            //pERROR("Failed to register hotkey %d\n", id);
            //error("Failed to register hotkey");
            //
            if (id == VK_HOTKEY1 || id == VK_HOTKEY7) 
                errflag = 0; // handle error if could not register a primary key
        }
    }
    return errflag;
}
