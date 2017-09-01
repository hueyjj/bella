# ifndef __BELLA_H
# define __BELLA_H

# include <Windows.h>
# include <Shellapi.h>
# include <time.h>

# ifdef DEBUGPRINT
# define pDEBUG(...) printf(__VA_ARGS__)
# define pERROR(...) fprintf(stderr, __VA_ARGS__)
# else
# define pDEBUG(...) 
# define pERROR(...)
# endif

# define size(arr)  (sizeof(arr) / sizeof(arr[0])) 

# define VK_COPY        0x00

# define VK_HOTKEY1     0x21
# define VK_HOTKEY2     0x22
# define VK_HOTKEY3     0x23
# define VK_HOTKEY4     0x24
# define VK_HOTKEY5     0x25
# define VK_HOTKEY6     0x26
# define VK_HOTKEY7     0x27
# define VK_HOTKEY8     0x28
# define VK_HOTKEY9     0x29
# define VK_HOTKEY10    0x2A

# define VK_0   0x30
# define VK_1   0x31
# define VK_2   0x32
# define VK_3   0x33
# define VK_4   0x34
# define VK_5   0x35
# define VK_6   0x36
# define VK_7   0x37
# define VK_8   0x38
# define VK_9   0x39

# define VK_A   0x41
# define VK_B   0x42
# define VK_C   0x43
# define VK_D   0x44
# define VK_E   0x45
# define VK_F   0x46
# define VK_G   0x47
# define VK_H   0x48
# define VK_I   0x49
# define VK_J   0x4A
# define VK_K   0x4B
# define VK_L   0x4C
# define VK_M   0x4D
# define VK_N   0x4E
# define VK_O   0x4F
# define VK_P   0x50
# define VK_Q   0x51
# define VK_R   0x52
# define VK_S   0x53
# define VK_T   0x54
# define VK_U   0x55
# define VK_V   0x56
# define VK_W   0x57
# define VK_X   0x58
# define VK_Y   0x59
# define VK_Z   0x5A

#define SYSTEM_TRAY_ICON_ID     2800 
#define WM_TRAYICON             WM_USER + 1

HWND hwnd;
HMENU systemTrayMenu;
HINSTANCE hInst;
NOTIFYICONDATA systemTrayIcon;

static const struct timespec mssleep = { .tv_sec = 0, .tv_nsec = 100000000 }; // 100 ms

// Input to send (Ctrl+c copy key)
static const INPUT copykey[] = { 
    { INPUT_KEYBOARD, .ki = { .wVk = VK_LCONTROL,   .dwFlags = 0 }},
    { INPUT_KEYBOARD, .ki = { .wVk = VK_C,          .dwFlags = 0 }},
    { INPUT_KEYBOARD, .ki = { .wVk = VK_C,          .dwFlags = KEYEVENTF_KEYUP }},
    { INPUT_KEYBOARD, .ki = { .wVk = VK_LCONTROL,   .dwFlags = KEYEVENTF_KEYUP }},
};

typedef struct key {
    int keys[2];
} key;

static const key hotkeys[] = {
    [VK_COPY]    = { .keys = { VK_CONTROL, VK_C }},
    [VK_HOTKEY1] = { .keys = { VK_CONTROL, VK_1 }},
    [VK_HOTKEY2] = { .keys = { VK_CONTROL, VK_2 }},
    [VK_HOTKEY3] = { .keys = { VK_CONTROL, VK_3 }},
    [VK_HOTKEY4] = { .keys = { VK_CONTROL, VK_4 }},
    [VK_HOTKEY5] = { .keys = { VK_CONTROL, VK_5 }},
    [VK_HOTKEY6] = { .keys = { VK_CONTROL, VK_OEM_3 }},
    [VK_HOTKEY7] = { .keys = { VK_CONTROL, VK_F1 }},
    [VK_HOTKEY8] = { .keys = { VK_CONTROL, VK_9 }},
};

/* Pop up message box. Used for anything not just error, but default use is error */
static void error(const char *msg)
{
    MessageBox(NULL, msg, "error", MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
}

static void printLastError(char *helpermsg)
{
    DWORD errorMessageID = GetLastError();
    if (errorMessageID != 0)
    {
        LPSTR message = NULL;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                     FORMAT_MESSAGE_FROM_SYSTEM |
                                     FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID,
                                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                                     (LPSTR)&message, 0, NULL);
        
        //pDEBUG("Last error: %s\n", message);
        char buffer[2056];
        sprintf(buffer, "%s %s", helpermsg, message);
        error(buffer);

        LocalFree(message);
    }
}

static void balloonTip(char *msg)
{
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = SYSTEM_TRAY_ICON_ID;
    nid.uFlags = NIF_INFO;
    nid.uCallbackMessage = WM_TRAYICON;
    //nid.hIcon = (HICON) LoadImage(NULL, TEXT("Happy-face.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    strcpy(nid.szInfo, TEXT(msg));
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

int isHotkeyDown(int hotkey[]);

void downloadhl(void);
void downloadClipboard(void);
void downloadBellaslist(void);
void storeHl(void);
void storeClipboard(void);

int registerHotkeys(HWND hwnd);

# endif
