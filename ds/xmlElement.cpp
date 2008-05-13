//#include "ddodefines.h"
#include "xmlElement.h"

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
        


XmlElement::XmlElement()
{
    mParent = NULL;
    
}

XmlElement::~XmlElement()
{
    clear();
}

XmlElement* XmlElement::getParent() const
{
    return mParent;
}

void XmlElement::setParent(XmlElement* parent)
{
    mParent = parent;
}

void XmlElement::addChild(XmlElement* child)
{
    child->setParent(this);
    mChildren.push_back(child);
}

size_t XmlElement::getNumChildren()
{
    return mChildren.size();
}

// This parse implementation supports nested elements of arbitrary
// types.  The value of this XmlElement will be the string
// representation of the internal XML elements or text. All child
// elements are also parsed and created as children of this object.
Result XmlElement::parse(string const& xml)
{
    XmlElement* curr = NULL;
    TokenStackMap tokenStackMap;
    
    size_t s=0;
    size_t e=0;
    string currTag;
    string openTag;
    
	// Skip the XML header if it exists.
	s = xml.find("?>", s);
	if(s == string::npos)
		s = 0;
	else
		s+=2;

    while(true)
    {
        s = xml.find("<",s);
        if( s == string::npos )
        {
            return Error;
        }
        
        s++;
        
        // Find the first whitespace char or end of tag.
        e = xml.find_first_of(" \t\r\n>", s );
        if( e == string::npos )
        {
            return Error;
        }
        
        currTag = xml.substr(s, e-s);
    
        TokenStackMap::iterator tsmi = tokenStackMap.find(currTag);
        
        // New token, add.
        if(tsmi == tokenStackMap.end())
        {
            pair<TokenStackMap::iterator, bool> ret =
                tokenStackMap.insert(TokenStackMap::value_type(currTag, new TokenStack));
            if(ret.second == false)
            {
                return Error;
            }
            
            tsmi = ret.first;
        }
        
        tsmi->second->push(new Token(s, e));
        
        // Check if this is a close token, and if so, match.
        if(currTag[0] == '/')
        {
            openTag = currTag.substr(1);
            TokenStackMap::iterator otsmi = tokenStackMap.find(openTag);
            if(otsmi == tokenStackMap.end())
            {
                return Error;
            }
        
            Token* openToken = otsmi->second->top();
            otsmi->second->pop();
            Token* closeToken = tsmi->second->top();
            tsmi->second->pop();
            
            // XML between the two tags is the value.
            size_t valS = openToken->mE;
            size_t valE = closeToken->mS;

            delete openToken;
            delete closeToken;

            // Check for attributes.
            if(xml[valS] != '>')
            {
                size_t attE = xml.find(">", valS);
                if(curr->parseAttributes(xml, valS, attE) < 0)
                {
                    return Error;
                }
                valS = attE;
            }
            
            // Set the value, skipping the > and </ .
            curr->setValue( xml.substr( valS+1, valE-valS-2 ) );
            

            // Move curr back to the parent.
            curr = curr->getParent();
            // Done. Parsed back to the original element.
            if(curr == NULL)
                return OK;
        }

        // This is an open token. Create a new XmlElement and update.
        else
        {
            // If curr isn't set, this is the first element, so use this.
            // I hate special conditions like this one.
            if(curr == NULL)
                curr = this;
            else
            {
                XmlElement* n = new XmlElement;
                curr->addChild(n);
                curr = n;
            }

            curr->setName(currTag);
        }

    }

    return Error;
}

void XmlElement::clear()
{
    mName.clear();
    mValue.clear();
    
    mAttribMap.clear();

    for(XmlElementVec::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
        delete *i;
    }

    mChildren.clear();
}


Result XmlElement::parseAttributes(string const& xml, size_t s, size_t e )
{
	string name;
	string val;
	pair<stringstringmap_t::iterator,bool> ret;

	while(true)
	{
		// find first non-whitespace char
		s = xml.find_first_not_of(" \t\r\n", s );
		if( s == string::npos )
			return Error;

		// found end of tag, done
		val = xml.at(s);
		if( !strcmp(val.c_str(),">")  )
			return OK;

		// find eql sign
		e = xml.find("=", s );
		if( e == string::npos )
			return Error;

		// get the attrib name
		name = xml.substr( s, e-s );
		s = e;

		// find first "
		s = xml.find("\"",s );
		if( s == string::npos )
			return Error;
		s++;

		// find second "
		e = xml.find("\"",s );
		if( e == string::npos )
			return Error;

		// get value
		val = xml.substr( s, e-s );

		// add pair
		setAttribute( name, val );
		s = e+1;
	}

    return OK;
}


Result XmlElement::setAttribute( const char* szName, const char* szVal )
{
	string n, v;
	n = szName;
	v = szVal;
	return setAttribute( n, v );
}

Result XmlElement::setAttribute( std::string const& name, std::string const& val )
{
	pair<stringstringmap_t::iterator,bool> ret;
	ret = mAttribMap.insert( stringstringmap_t::value_type(name,val) );
	if( ret.second == false )
	{
		// attrib exists, update value
		ret.first->second = val;
	}
	return OK;
}

std::string const* XmlElement::getAttributeValue( std::string const& name ) const
{
	stringstringmap_t::const_iterator i = mAttribMap.find( name );
	if( i != mAttribMap.end() )
		return &(i->second);

	return NULL;
}

std::string const* XmlElement::getAttributeValue( char const* szName ) const
{
	string n = szName;
	return getAttributeValue( n );
}

Result XmlElement::setXml(string const& xml)
{
    return parse(xml);
}

Result XmlElement::setXml(char const* xml)
{
    string strXml = xml;
    return parse(strXml);
}

XmlElement& XmlElement::operator=(XmlElement const& rh)
{
    string xml;
    rh.getXml(xml);
    parse(xml);
    return *this;
}


void XmlElement::getXml(string& xml) const
{
    xml.clear();
    xml = "<";
    xml+= mName;
    for(stringstringmap_t::const_iterator i = mAttribMap.begin(); i!=mAttribMap.end(); ++i)
    {
        xml+= " ";
        xml+= i->first;
        xml+= "=\"";
        xml+= i->second;
        xml+= "\"";
    }

    xml+=">";
    xml+= mValue;
    xml+="</";
    xml+=mName;
    xml+=">";
}

    


    
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
