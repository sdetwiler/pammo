#include "object.h"
#include "db/query.h"
#include "field.h"
#include "fieldInt.h"
#include "fieldString.h"

#include <string>

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

Object::Object(db::Database* db)
{
    mDb = db;
    mId = 0;
    mOid = 0;
    mCreatedTime = 0;
    mModifiedTime = 0;
    mDbState = NotStored;
}

Object::~Object()
{}

Object& Object::operator=(Object const& o)
{
    mData = o.mData;
    mOid = o.mOid;
    mDb = o.mDb;
    mId = 0;
    mCreatedTime = 0;
    mModifiedTime = 0;
    mDbState = NotStored;
    
    return *this;
}


string const* Object::getName()
{
    return mData.getName();
}

Result Object::load(Id id)
{
    db::Query* query = mDb->newQuery();
    char q[128];
    sprintf(q, "SELECT * FROM object WHERE id=%d", id);
    
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

    // TODO: The ObjectTemplate should be loaded first to pull in any
    // new field changes prior to loaded stored values.

    // Load stored values.
    string const* val;
    Field* f;

    f = t->getField("id");
    if(!f || f->getType() != Int)
    {
        return Error;
    }
    mId = ((FieldInt*)f)->getIntValue();


    
    f = t->getField("oid");
    if(!f || f->getType()!=Int)
    {
        return Error;
    }
    mOid = ((FieldInt*)f)->getIntValue();

    //createdTime
    //modifiedTime
    
    f = t->getField("data");
    if(!f || f->getType()!=String)
    {
        return Error;
    }
    mData.loadFromXml(((FieldString*)f)->getStringValue());


    
    mDbState = Fresh; // This object is already in DB and therefore OK.
    return OK;
}


Result Object::save()
{
    // If the object is new, add it.
    if(mDbState == NotStored)
        return add();
    
    string xml;
    if(mData.getAsXml(xml) < 0)
    {
        return Error;
    }

    db::Query* query = mDb->newQuery();
    time_t now = time(NULL);

    string q = "UPDATE object SET modifiedTime='";
    q+= ctime(&now);
    q+= "', data ='";
    q+=xml;
    q+="'";
    
    
    query->setQuery(q);
    if(query->execute() < 0)
    {
        delete query;
        return Error;
    }
    delete query;

    mDbState = Fresh;
    
    return OK;
}

Result Object::add()
{
    string xml;
    if(mData.getAsXml(xml) < 0)
    {
        return Error;
    }

    db::Query* query = mDb->newQuery();
    time_t now = time(NULL);
    char buf[64];
    string q = "INSERT INTO object VALUES (0,";
    sprintf(buf, "%d", mOid);
    
    q+=buf; // oid
    q+=", '";
    q+=ctime(&now); // createdTime
    q+="', '";
    q+=ctime(&now); // modifiedTime
    q+="', '";
    q+=xml;        // data.
    q+="'";

    query->setQuery(q);
    if(query->execute() < 0)
    {
        delete query;
        return Error;
    }
    delete query;

    mDbState = Fresh; // The object is no longer new since it is now in the database.
    
    return OK;
}

Field* Object::getField(char const* name)
{
    return mData.getField(name);
}

Field* Object::getField(std::string const& name)
{
    return mData.getField(name.c_str());
}

Id Object::getId()
{
    return mId;
}

Id Object::getObjectId()
{
    return mOid;
}

    
    
    
    

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
