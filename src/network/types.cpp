#include "types.h"
#include <stdio.h>

void hexdump(char const* str, void* buf, uint32_t bufLen)
{
    printf("%s:\n", str);
    
    for(uint32_t i=0; i<bufLen; ++i)
    {
        printf("%02x ", *(((uint8_t*)buf)+i));
        if(i && !(i%20))
            printf("\n");
    }

    printf("\n");
}

