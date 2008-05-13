///////////////////////////////////////////////////////////////
// fieldInt.h
//
// Integer DataStore field.
//
///////////////////////////////////////////////////////////////

#ifndef PS_DS_FIELDINT_H
#define PS_DS_FIELDINT_H

#include "field.h"

#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

class FieldInt : public Field
{
    public:
        FieldInt();
        virtual ~FieldInt();

        // Sets value of field by int.
        virtual void setValue(int32_t val);
        // Sets value of field by string.
        virtual void setValue(char const* val);
        // Sets value of field by string.
        virtual void setValue(std::string const& val);
        // Get unique type id for field.
        virtual Type getType() const;
        // Get the field's value encoded as a string.
        virtual void getStringEncValue(std::string& val) const;
        virtual int32_t getIntValue() const;
        
    private:
        int32_t mVal;
        
};
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif
