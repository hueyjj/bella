# bella
bella maps a few global hotkeys to execute some youtube-dl commands.

    Usage: ./bella.exe 
    Default save directory: ./current_saves (for single file saves)
    Default save all directory: ./bellaslist_all (for downloading all from default save file)
    Default save file: ./bellaslist.txt
    Default bindings:
        hotkey1     <control 1>    Download media from highlighted url.
        hotkey2     <control 2>    Download media from url in clipboard.
        hotkey3     <control 3>    Download all in ./bellaslist.txt
        hotkey4     <control 4>    Store highlighted content to ./bellaslist.txt
        hotkey5     <control 5>    Store clipboard content to ./bellaslist.txt
        hotkey6     <control `>    Exit bella
        hotkey7     <control F1>   Exit bella
        hotkey8     <control 9>    Check if bella is running 

bella isn't a very strong girl, so please don't ask her to do something she can't. 
For bella to work she needs...
1. win32 api (Windows)
2. youtube-dl (runnable through shell, and path to C:\ffmpeg\bin\youtube-dl, 
or change it directly in the source file, bella.c)


    static const char *youtube = C:\\ffmpeg\\bin\\youtube-dl

to

    static const char *youtube = C:\\path\\to\\youtube-dl

## Compiling
If there is the 'make' command, then use the provided makefile and hope it works.
Otherwise it is possible to use Visual studio as bella is really just two .c files, one header file, and one rc file (for the icon).
Or use a more comfortable tool is possible.

##### who is bella
Just a quick, dirty program to download youtube videos using youtube-dl without having to enter
stuff into a command line / shell. Bella basically does two things. Get stuff from the system clipboard
(where control + c content lives) and whatever is highlighted. Then it just passes that information to 
youtube-dl and let it do the hard work.

Usually this how it would be used:
1. Run bella.exe and hope that no dialog pop up (errors)
2. Control + 9 to check if a balloon tip comes up
3. Highlight link in the browser
4. Control + 1
5. Check if "downloading" balloon tip comes up
6. Wait for "complete" balloon tip to come up
7. If necessary, check the downloads folder in one of the default folder to see if the file is there
