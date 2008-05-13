#include "fieldFactory.h"
#include "table.h"

#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
 
Table::Table()
{}


Table::~Table()
{
    clear();
}



Table& Table::operator=(Table const& t)
{
    string v;
    t.getAsXml(v);
    loadFromXml(v);
    return *this;
}

Result Table::getAsXml(string& xml) const
{
	xml = "<table";

    // Future support for base64 encoding.
    // xml+= "enctype=\"plain\" ";

    stringstringmap_t attribMap = mXml.getAtributeMap();
    for( stringstringmap_t::const_iterator i = attribMap.begin(); i!=attribMap.end(); i++ )
    {
        xml+= " ";
        xml+= i->first.c_str();
        xml+= "=\"";
        xml+= i->second.c_str();
        xml+= "\"";
    }

	xml+=">";

    // Get XML encoding for all fields in table.
	string val;
    for(fieldvec_t::const_iterator fvi = mFields.begin(); fvi != mFields.end(); fvi++)
    {
        (*fvi)->getAsXml(val);
        xml+="\n";
        xml+=val;
    }
	xml+="\n</table>";

	return OK;
}


Result Table::loadFromXml(string const& xml)
{
    if(mXml.setXml(xml) < 0)
    {
        return Error;
    }
    

    return loadFromXml(mXml);
}

// Expects field elements as children. Field elements cannot contain other field elements.
Result Table::loadFromXml(XmlElement const& xml)
{
    // for each <field> in value, add a field to this table.
	// get the value and set it.

    size_t s=0;
    size_t e;
    
    while(true)
    {
        s = xml.getValue().find("<field", s);
        if(s == string::npos)
            return OK;

        e = xml.getValue().find("</field>", s);
        if(e == string::npos)
        {
            printf("failed to find </field>\n");
            return Error;
        }
        

        e+=8;  //strlen("</field>");
        if(addField(xml.getValue().substr(s, e-s)) < 0)
        {
            return Error;
        }
        

        s=e;
    }
    
    return OK;
}   



Result Table::loadFromFile(char const* filename)
{
    ifstream infile;
    infile.open(filename, ios_base::in);
    if(!infile.good())
        return Error;

    string xml;
    char buf[4096];
    size_t nRead;
    streamsize ss;
    
    while(true)
    {
        infile.read(buf, sizeof(buf));
        nRead = infile.gcount();
        if(nRead)
        {
            buf[nRead]=0;
            xml+=buf;
        }
        
        if(nRead < sizeof(buf))
            break;
    }

    infile.close();

    return loadFromXml(xml);
}


Result Table::saveToFile(char const* filename)
{
    string xml;
    if(getAsXml(xml)<0)
        return Error;

    ofstream outfile;
    outfile.open(filename, ios_base::trunc);
    if(!outfile.good())
        return Error;


    outfile.write(xml.c_str(), xml.length()+1);
    outfile.close();

    return OK;
}

    
    
    

string const* Table::getName()
{
    return getAttributeValue("name");
    
}


void Table::setName(string const& name)
{
    setAttribute("name", name.c_str());
}


void Table::setName(char const* name)
{
    setAttribute("name", name);
}

string const* Table::getAttributeValue(char const* attribName) const
{
    return mXml.getAttributeValue(attribName);
}

string const* Table::getAttributeValue(string const& attribName) const
{
    return mXml.getAttributeValue(attribName);
}


Result Table::setAttribute(string const& attribName, string const& val)
{
    return mXml.setAttribute(attribName, val);
}


Result Table::setAttribute(char const* attribName, char const* val)
{
    return mXml.setAttribute(attribName, val);
}


void Table::clear()
{
    for(fieldvec_t::iterator fvi = mFields.begin(); fvi != mFields.end(); fvi++)
    {
        delete *fvi;
    }

    mFieldsByName.clear();
    mFields.clear();
    

    
    mXml.clear();
}

Result Table::addField(string const& xml)
{
    size_t s = xml.find(" type=\"", 0);
    if(s == string::npos)
    {
        return Error;
    }
    

    s+=7;   // strlen(" type=\"")
    size_t e = xml.find("\"", s);
    if(e == string::npos)
    {
        return Error;
    }
    
    string strType = xml.substr(s, e-s);
    
    Type t = (Type)atol(strType.c_str());
    
    Field* f = FieldFactory::create(t);
    if(!f)
    {
        return Error;
    }
    
    if(f->loadFromXml(xml) < 0)
    {
        delete f;
        return Error;
    }

    return addField(f);
}

    

Result Table::addField(Field* f)
{
    string const* name = f->getName();
    
    pair<stringfieldmap_t::iterator, bool> ret;
    ret = mFieldsByName.insert(stringfieldmap_t::value_type(*name, f));
    // Insert failed: Field with name already exists. Update existing field with new data.
    if(ret.second == false)
    {
        *(*(ret.first)).second = *f;
        
        delete f;
        return OK;
    }
    
    // Add field to end of list.
    mFields.insert(mFields.end(), f);

    return OK;
}

Field* Table::getField(char const* name) const
{
    string n = name;
    
    stringfieldmap_t::const_iterator i = mFieldsByName.find(n);
    if(i != mFieldsByName.end())
        return i->second;

    return NULL;
}

Field* Table::getField(size_t n) const
{
    if(n >= mFields.size())
        return NULL;

    return mFields[n];
}


size_t Table::getFieldCount() const
{
    return mFields.size();
}

    
Result Table::removeField(char const* name)
{
    Field* f;
    
    string n = name;
    
    stringfieldmap_t::iterator sfmi = mFieldsByName.find(n);
    if(sfmi == mFieldsByName.end())
        return Error;

    f = sfmi->second;
    
    mFieldsByName.erase(sfmi);

    for(fieldvec_t::iterator fvi = mFields.begin(); fvi != mFields.end(); fvi++)
    {
        if(*fvi == f)
        {
            mFields.erase(fvi);
            break;
        }
    }

    delete f;
    return OK;
}

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
