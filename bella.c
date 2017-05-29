# include <stdbool.h>

# include "bella.h"

static void copy(void);

static const char savedir[] = "/saves/";
static const short mostsigbit = (1 << 15);

static void copy(void)
{
    
}

int iskeydown(int hotkey[])
{
    for (int i = 0; i < size(hotkey); ++i)
    {
        if ( (short)(GetAsyncKeyState(hotkey[i]) & mostsigbit) == mostsigbit)
        {
            return true; // Key is down
        }
    }
    return false;
}

int download_hlurl(void)
{
}

int download_clipbd_url(void)
{
}

int download_all(void)
{
}

int store_hlurl(void)
{
}

int store_hlurl_clipbd(void)
{
}

int build_hotkeys(void)
{
    int exitflag = 0;

    if ( !RegisterHotKey(NULL, VK_HOTKEY1, MOD_ALT, VK_1) )
    {
        fprintf(stderr, "Failed to register hotkey 1\n");
        exitflag = 1;
    }
    
    if ( !RegisterHotKey(NULL, VK_HOTKEY2, MOD_ALT, VK_2) )
    {
        fprintf(stderr, "Failed to register hotkey 2\n");
        exitflag = 1;
    }

    if ( !RegisterHotKey(NULL, VK_HOTKEY3, MOD_ALT, VK_3) )
    {
        fprintf(stderr, "Failed to register hotkey 3\n");
        exitflag = 1;
    }

    if ( !RegisterHotKey(NULL, VK_HOTKEY4, MOD_ALT, VK_4) )
    {
        fprintf(stderr, "Failed to register hotkey 4\n");
        exitflag = 1;
    }

    if ( !RegisterHotKey(NULL, VK_HOTKEY5, MOD_ALT, VK_5) )
    {
        fprintf(stderr, "Failed to register hotkey 5\n");
        exitflag = 1;
    }

    return exitflag;
}
