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

static void error(const char *);
static int  copy(void);
static char *getclipboardS(void);
static void download(const char *, const char *);
static void store(const char *);

static const char savedir[] = "./saves";
static const char savedir2[] = "./bellascandy";
static const char savefile[] = "./bellaslist.txt";
static const short mostsigbit = (1 << 15);

static void error(const char *msg)
{
    MessageBox(NULL, msg, "error", MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
}

/* Copy to clipboard only if it's different than the previous content*/
static int copy(void)
{
    UINT result;
    const int maxretry = 5;
    int retry = 0;
    char prev[MAX_SZ], curr[MAX_SZ];

    //prev = getclipboardS();
    strcpy(prev, getclipboardS());

    do 
    {
        //pDEBUG("prev: %s\n", prev);
        //pDEBUG("curr: %s\n", curr);
        retry++;
        result = SendInput(4, (LPINPUT) copykey, sizeof(INPUT));
        for (int i = 0; i < 2; ++i) nanosleep(&mssleep, NULL); // 200 ms
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

static void download(const char *dir, const char *url)
{
    struct stat sb = {0};
    if (stat(dir, &sb) != 0 || !S_ISDIR(sb.st_mode))
        mkdir(dir);

    //char *cmd;
    const char *youtube = "youtube-dl";
    const char *param = "-o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata";
    char *paramb;

    asprintf(&paramb, "%s %s", param, url);

    //const char *youtubedl = "youtube-dl -o \"%(title)s.%(ext)s\" -i --format m4a --embed-thumbnail --add-metadata";
    //asprintf(&cmd, "(cd %s && %s %s)", dir, youtubedl, url);
    //pDEBUG("\nExecuting...\n%s\n\n", cmd); 


    pDEBUG("\nExecuting..\n\n");
    int sh = ShellExecute(NULL, "open", youtube, paramb, dir, SW_HIDE);
    if (sh <= 32)
    {
        error("download: Unable to execute youtube-dl"); 
        return;
    }
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
        error("download_hl: Failed to copy");
        return;
    }

    // TODO add a way to verify if downloaded and if it's the right file

    char *url = getclipboardS();
    pDEBUG("In clipboard: %s\n", url);
    download(savedir, url);
}

void download_clipboard(void)
{
    char *url = getclipboardS();
    download(savedir, url);
}

void download_bellaslist(void)
{
    FILE *infile;

    if ( (infile = fopen(savefile, "r")) == (FILE *) 0)
    {
        pERROR("download_bellaslist: Unable to open bellaslist\n");
        error("download_bellaslist: Unable to open bellaslist");
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

void store_hl(void)
{
    copy();

    char buffer[MAX_SZ];
    strcpy(buffer, getclipboardS());

    store(buffer);
}

void store_clipboard(void)
{
    store(getclipboardS()); 
}

int reghotkeys(void)
{
    int exitflag = 0;

    if (!RegisterHotKey(NULL, VK_HOTKEY1, MOD_CONTROL, hotkeys[VK_HOTKEY1].keys[1]))
    {
        pERROR("Failed to register hotkey 1\n");
        exitflag = 1;
    }
    
    if (!RegisterHotKey(NULL, VK_HOTKEY2, MOD_CONTROL, hotkeys[VK_HOTKEY2].keys[1]))
    {
        pERROR("Failed to register hotkey 2\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY3, MOD_CONTROL, hotkeys[VK_HOTKEY3].keys[1]))
    {
        pERROR("Failed to register hotkey 3\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY4, MOD_CONTROL, hotkeys[VK_HOTKEY4].keys[1]))
    {
        pERROR("Failed to register hotkey 4\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY5, MOD_CONTROL, hotkeys[VK_HOTKEY5].keys[1]))
    {
        pERROR("Failed to register hotkey 5\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY6, MOD_CONTROL, hotkeys[VK_HOTKEY6].keys[1]))
    {
        pERROR("Failed to register hotkey 6\n");
        exitflag = 1;
    }

    if (!RegisterHotKey(NULL, VK_HOTKEY7, MOD_CONTROL, hotkeys[VK_HOTKEY7].keys[1]))
    {
        pERROR("Failed to register hotkey 7\n");
        exitflag = 1;
    }
    return exitflag;
}
