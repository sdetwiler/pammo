#include "objectTemplate.h"
#include "object.h"
#include "db/query.h"

#include "fieldInt.h"
#include "fieldString.h"

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

ObjectTemplate::ObjectTemplate()
{
    mDb = NULL;
    mOid = 0;
}


ObjectTemplate::~ObjectTemplate()
{
    //
}

void ObjectTemplate::setDatabase(db::Database* d)
{
    mDb = d;
}


std::string const* ObjectTemplate::getName()
{
    return mData.getName();
}

Id ObjectTemplate::getObjectId()
{
    return mOid;
}

Result ObjectTemplate::load(Id id)
{
    if(!mDb)
        return Error;
    
    db::Query* query = mDb->newQuery();
    char q[128];
    sprintf(q, "SELECT * FROM objecttemplate WHERE oid=%d", id);
    
    query->setQuery(q);
    if(query->execute() < 0)
    {
        delete query;
        return Error;
    }

    if(query->saveResults() < 0)
    {
        delete query;
        return Error;
    }
    // Get first result.
    Table* t = query->getResult(0);
    if(!t)
    {
        delete query;
        return Error;
    }

    delete query;

    return load(t);
}

Result ObjectTemplate::load(Table const* table)
{
    string const* val;
    Field* f;
    f = table->getField("oid");
    if(!f || f->getType() != Int)
    {
        return Error;
    }
    mOid = ((FieldInt*)f)->getIntValue();
    
    // createdTime.
    // modifiedTime.

    f = table->getField("data");
    if(!f || f->getType()!=String)
    {
        return Error;
    }
    mData.loadFromXml(((FieldString*)f)->getStringValue());

    return Error;
    
}

    


Object* ObjectTemplate::newObject()
{
    if(!mDb)
        return NULL;
    
    Object* o = new Object(mDb);
    o->mData = mData;
    return o;
}

Field* ObjectTemplate::getField(char const* name)
{
    return mData.getField(name);
}

Field* ObjectTemplate::getField(std::string const& name)
{
    return mData.getField(name.c_str());
}

Result ObjectTemplate::addField(Field* f)
{
    return mData.addField(f);
}



///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

    
