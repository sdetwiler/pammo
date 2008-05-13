#ifndef PS_TYPES_H
#define PS_TYPES_H

enum Result
{
    OK=0,
    Error=-1,
};


enum Type
{
    Unknown = 0,
    String =  1,
    Int =     2,
};

#include <stdint.h>
//typedef unsigned long uint32_t;

typedef uint32_t Id;



#endif
