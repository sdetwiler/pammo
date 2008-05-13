///////////////////////////////////////////////////////////////
// field.h
//
// Base class for DataStore fields.
//
///////////////////////////////////////////////////////////////

#ifndef PS_DS_FIELD_H
#define PS_DS_FIELD_H

#include "types.h"
#include "xmlElement.h"

#include <string>
///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

class Field
{
    public:
        Field();
        virtual ~Field();

        Field& operator=(Field const& f);

        virtual Result getAsXml(std::string& xml) const;

        Result loadFromXml(std::string const& xml);
        Result loadFromXml(XmlElement const& xml);

        virtual std::string const* getName() const;
        virtual void setName(std::string const& name);
        virtual void setName(char const* name);

        std::string const* getAttributeValue(char const* attribName) const;
        std::string const* getAttributeValue(std::string const& attribName) const;

        Result setAttribute(std::string const& attribName, std::string const& val);
        Result setAttribute(char const* attribName, char const* val);

        virtual void clear();
        
        // Sets value of field by string.
        virtual void setValue(char const* val)=0;
        // Sets value of field by string.
        virtual void setValue(std::string const& val)=0;
        // Get unique type for field.
        virtual Type getType() const =0;
        // Get the field's value encoded as a string.
        virtual void getStringEncValue(std::string& val) const=0;

    protected:

    private:
        XmlElement  mXml;
};

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif    // PS_DS_FIELD_H
