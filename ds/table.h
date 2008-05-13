///////////////////////////////////////////////////////////////
// table.h
///////////////////////////////////////////////////////////////
#ifndef PS_DS_TABLE_H
#define PS_DS_TABLE_H

#include "field.h"

#include <string>
#include <map>
#include <vector>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
        
typedef std::map<std::string, Field*> stringfieldmap_t;
typedef std::vector<Field*> fieldvec_t;


class Table
{
    public:
        Table();
        virtual ~Table();

        Table& operator=(Table const& t);

        virtual Result getAsXml(std::string& xml) const;

        Result loadFromXml(std::string const& xml);
        Result loadFromXml(XmlElement const& xml);

        Result loadFromFile(char const* filename);

        Result saveToFile(char const* filename);
        

        virtual std::string const* getName();
        virtual void setName(std::string const& name);
        virtual void setName(char const* name);

        std::string const* getAttributeValue(char const* attribName) const;
        std::string const* getAttributeValue(std::string const& attribName) const;

        Result setAttribute(std::string const& attribName, std::string const& val);
        Result setAttribute(char const* attribName, char const* val);

        void clear();
        
        // Add field to table. Field will be dealloc'd internally.
        Result addField(Field* f);

        Result addField(std::string const& xml);

        Field* getField(char const* name) const;
        /// Returns the n'th field in the table.
        Field* getField(size_t n) const;

        /// Returns the number of fields in the Table.
        size_t getFieldCount() const;
        
        
        Result removeField(char const* name);
        
        
    protected:

    private:
        XmlElement       mXml;
        
        stringfieldmap_t mFieldsByName;    // alpha order index
        fieldvec_t       mFields;          // schema order
};

typedef std::vector<Table*> TableVec;
    
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
#endif
