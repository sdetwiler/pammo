#include "field.h"


using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

Field::Field()
{
    
}


Field::~Field()
{}


Field& Field::operator=(Field const& f)
{
    string v;
    clear();
    
    f.getAsXml(v);
    loadFromXml(v);
    return *this;
}

Result Field::getAsXml(string& xml) const
{
	xml = "<field";

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
	string val;
	getStringEncValue(val);
    xml+= val;

	xml+="</field>";

	return OK;
}


Result Field::loadFromXml(string const& xml)
{
    if(mXml.setXml(xml) < 0)
        return Error;

    return loadFromXml(mXml);
}

Result Field::loadFromXml(XmlElement const& xml)
{
    char type[32];
    snprintf(type, 32, "%d", getType());
    setAttribute("type", type);



    // get the value and set it.
    setValue( xml.getValue() );

	return OK;
}   


string const* Field::getName() const
{
    return getAttributeValue("name");
    
}


void Field::setName(string const& name)
{
    setAttribute("name", name.c_str());
}


void Field::setName(char const* name)
{
    setAttribute("name", name);
}

string const* Field::getAttributeValue(char const* attribName) const
{
    return mXml.getAttributeValue(attribName);
}

string const* Field::getAttributeValue(string const& attribName) const
{
    return mXml.getAttributeValue(attribName);
}


Result Field::setAttribute(string const& attribName, string const& val)
{
    return mXml.setAttribute(attribName, val);
}


Result Field::setAttribute(char const* attribName, char const* val)
{
    return mXml.setAttribute(attribName, val);
}


void Field::clear()
{
    mXml.clear();
}

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
