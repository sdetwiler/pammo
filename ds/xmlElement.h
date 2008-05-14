#ifndef __PS_DS_XMLELEMENT_H__
#define __PS_DS_XMLELEMENT_H__

///////////////////////////////////////////////////////////////
// XmlElement.h
//
// This class provides basic XML Element support. It is not DOM
// compliant.
//
//
///////////////////////////////////////////////////////////////

#include "types.h"
//#include <vector>


///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

typedef std::map< std::string, std::string > stringstringmap_t;

class XmlElement
{
public:
	XmlElement();
	virtual ~XmlElement();

    XmlElement& operator=(XmlElement const& rh);
        
	Result setXml( std::string const& xml );
	Result setXml( char const* szXml );
    void getXml(std::string& xml) const;
        

    void clear();
        
    stringstringmap_t const& getAtributeMap() const{ return mAttribMap; }


	std::string const& getName() const{ return mName; }
	std::string const& getValue()const { return mValue; }

	Result setAttribute( std::string const & name, std::string const& val );
	Result setAttribute( const char* szName, const char* szVal );

    std::string const* getAttributeValue( std::string const& name ) const;
    std::string const* getAttributeValue( char const* name ) const;

    XmlElement* getParent() const;
    void addChild(XmlElement* child);
    size_t getNumChildren();
        
	void setName( std::string const& n ){ mName = n; }
	void setValue( std::string const& v ){ mValue = v; }
        

private:
    typedef std::vector< XmlElement* > XmlElementVec;
        

    std::string			mName;
	std::string			mValue;
	stringstringmap_t	mAttribMap;
    XmlElementVec       mChildren;
    XmlElement*         mParent;
        
    void setParent(XmlElement* parent);
        
        
    Result parse(std::string const& xml);
	Result parseAttributes( std::string const& xml, size_t s, size_t e );

    class Token
    {
        public:
            Token(size_t s, size_t e)
            {
                mS = s;
                mE = e;
            }
            
            
        size_t    mS;    // Start index, after <
        size_t    mE;    // End index, after <
    };

    typedef std::stack<Token*> TokenStack;
    class TokenStackMap : public std::map<std::string, TokenStack*>
    {
        public:
            virtual ~TokenStackMap()
            {
                for(TokenStackMap::iterator i = begin(); i != end(); ++i)
                {
                    delete i->second;
                }
            }
    };
};

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif
