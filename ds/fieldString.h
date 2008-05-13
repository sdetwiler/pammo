///////////////////////////////////////////////////////////////
// fieldString.h
//
// String DataStore field.
//
///////////////////////////////////////////////////////////////

#ifndef PS_DS_FIELDSTRING_H
#define PS_DS_FIELDSTRING_H

#include "field.h"

#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

class FieldString : public Field
{
    public:
        FieldString();
        virtual ~FieldString();

        // Sets value of field by string.
        virtual void setValue(char const* val);
        // Sets value of field by string.
        virtual void setValue(std::string const& val);
        // Get unique type id for field.
        virtual Type getType() const;
        // Get the field's value encoded as a string.
        virtual void getStringEncValue(std::string& val) const;
        // Get the value as string.
        virtual std::string const& getStringValue() const;
        
    private:
        std::string mVal;
        
};
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif
