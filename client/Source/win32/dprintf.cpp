#include "platform.h"
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <stdio.h>

namespace pammo
{

void dprintf(char const* format, ...)
{
    // Get a debug console for win32. Yea.
    if(AllocConsole())
    {
        freopen("CONOUT$", "wt", stdout);
        SetConsoleTitle(L"PAMMO : Debug Console");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
        // Truncate the file since this is a new process.
        FILE* f = fopen("pammo.txt", "w");
        fclose(f);
    }


	va_list args;
    int     len;
    char* buffer;

	va_start( args, format );

    len = _vscprintf(format, args)+1;
    buffer = (char*)_alloca(len);
    vsprintf( buffer, format, args );

    va_end(args);	

    FILE* f = fopen("pammo.txt", "a");
    if(!f)
        return;
    fwrite(buffer, 1, len, f);
    fwrite("\r\n", 1, 2, f);
    fclose(f);

    printf("%s\n", buffer);
}


} // namespace pammo