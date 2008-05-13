#include "fieldString.h"

using namespace std;
///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

FieldString::FieldString()
{
    char type[32];
    snprintf(type, 32, "%d", getType());
    setAttribute("type", type);
    mVal = "";
    
}

FieldString::~FieldString()
{}

Type FieldString::getType() const
{
    return String;
}

void FieldString::setValue(char const* val)
{
    mVal = val;
}

void FieldString::setValue(string const& val)
{
    mVal = val;
}

void FieldString::getStringEncValue(string& val) const
{
    val = mVal;
}

string const& FieldString::getStringValue() const
{
    return mVal;
}

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
