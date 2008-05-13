#include "objectTemplateIndex.h"
#include "db/query.h"

#include <string>

using namespace std;


///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
/////////////////////////////////////////////////////////////// 
ObjectTemplateIndex::ObjectTemplateIndex()
{}


ObjectTemplateIndex::~ObjectTemplateIndex()
{
    shutdown();
}
   

    
Result ObjectTemplateIndex::startup(db::Database* database)
{
    db::Query* query = database->newQuery();
    if(!query)
    {
        return Error;
    }

    query->setQuery("SELECT * FROM objecttemplate");
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

    // Each returned table is one ObjectTemplate.
    TableVec* tv = query->getResults();
    for(TableVec::iterator i = tv->begin(); i!=tv->end(); i++)
    {
        ObjectTemplate* ot = new ObjectTemplate;
        if(ot->load(*i) < 0)
        {
            delete ot;
        }
        else
        {
            // TODO: check return code and respond?
            addObjectTemplate(ot);
        }
        
            
    }
    
    return OK;
}

void ObjectTemplateIndex::shutdown()
{
    for(IdObjectTemplateMap::iterator i = mIdObjectTemplateMap.begin(); i!= mIdObjectTemplateMap.end(); ++i)
    {
        delete i->second;
    }

    mIdObjectTemplateMap.clear();
    mNameObjectTemplateMap.clear();
}


Result ObjectTemplateIndex::addObjectTemplate(ObjectTemplate* ot)
{
    string const* name = ot->getName();
    if(!name)
    {
        delete ot;
        return Error;
    }

    mIdObjectTemplateMap.insert(IdObjectTemplateMap::value_type(ot->getObjectId(), ot));
    mNameObjectTemplateMap.insert(StringObjectTemplateMap::value_type(*name, ot));
    return OK;
}


ObjectTemplate* ObjectTemplateIndex::getObjectTemplate(Id oid)
{
    IdObjectTemplateMap::iterator i = mIdObjectTemplateMap.find(oid);
    if(i == mIdObjectTemplateMap.end())
        return NULL;

    return i->second;
}

ObjectTemplate* ObjectTemplateIndex::getObjectTemplate(string& name)
{
    StringObjectTemplateMap::iterator i = mNameObjectTemplateMap.find(name);
    if(i == mNameObjectTemplateMap.end())
        return NULL;

    return i->second;
}

ObjectTemplate* ObjectTemplateIndex::getObjectTemplate(char const* name)
{
    string n = name;
    return getObjectTemplate(n);
}




///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
