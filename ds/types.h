#ifndef __PS_TYPES_H__
#define __PS_TYPES_H__

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

typedef uint32_t Id;

// SCD WTF.
#include <vector>
#include <string>
#include <map>
#include <stack>

#endif
