#include "fieldInt.h"

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

FieldInt::FieldInt()
{
    char type[32];
    snprintf(type, 32, "%d", getType());
    setAttribute("type", type);

    mVal = 0;
}

FieldInt::~FieldInt()
{}

Type FieldInt::getType() const
{
    return Int;
}

void FieldInt::setValue(int32_t val)
{
    mVal = val;
}

void FieldInt::setValue(char const* val)
{
    mVal = atol(val);
}

void FieldInt::setValue(string const& val)
{
    mVal = atol(val.c_str());
}

int32_t FieldInt::getIntValue() const
{
    return mVal;
}


void FieldInt::getStringEncValue(string& val) const
{
    char buf[24];
    snprintf(buf, 24, "%d", mVal);
    val = buf;
}
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
