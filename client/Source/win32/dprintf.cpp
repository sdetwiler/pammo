#include "dprintf.h"
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>

void dprintf(char const* format, ...)
{
	va_list args;
    int     len;
    char* buffer;

	va_start( args, format );

    len = _vscprintf(format, args)+1;
    buffer = (char*)_alloca(len);
    vsprintf( buffer, format, args );

    va_end(args);	
}
