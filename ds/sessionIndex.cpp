#include "sessionIndex.h"
#include "dataStore.h"

#include "db/server.h"


#include <string>

using namespace std;


///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
/////////////////////////////////////////////////////////////// 
SessionIndex::SessionIndex()
{}


SessionIndex::~SessionIndex()
{
    shutdown();
}
   

    
Result SessionIndex::startup(DataStore* datastore)
{
    mDs = datastore;


    // Add the guest user session.
    Session* s = createSession("", "");
    if(!s)
        return Error;
    
    returnSession(s);
    
    
    
    /*** For now, sessions will not restore to memory on startup
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

    // Each returned table is one Session.
    TableVec* tv = query->getResults();
    for(TableVec::iterator i = tv->begin(); i!=tv->end(); i++)
    {
        Session* ot = new Session;
        if(ot->load(*i) < 0)
        {
            delete ot;
        }
        else
        {
            // TODO: check return code and respond?
            addSession(ot);
        }
        
            
    }
    **/
    
    return OK;
}

void SessionIndex::shutdown()
{
    for(IdSessionMap::iterator i = mIdSessionMap.begin(); i!= mIdSessionMap.end(); ++i)
    {
        delete i->second;
    }

    mIdSessionMap.clear();
    mNameSessionMap.clear();
}


Session* SessionIndex::createSession(char const* username, char const* password)
{

    // This will prevent the same user from concurrently logging more
    // than once. Is that what is desired?
    /**
    Session* s = getSession(username);
    if(s)
        return s;
    **/
    
    Session* s = new Session(mDs);
    // validate username/password here.
    if(s->loginUser(username, password) < 0)
    {
        delete s;
        return NULL;
    }

    // Add to maps.
    string strName = username;
    mIdSessionMap.insert(IdSessionMap::value_type(s->getId(), s));
    mNameSessionMap.insert(StringSessionMap::value_type(strName, s));

    return s;
}


Session* SessionIndex::getSession(Id sid)
{
    IdSessionMap::iterator i = mIdSessionMap.find(sid);
    if(i == mIdSessionMap.end())
        return NULL;
    
    return i->second;
}


void SessionIndex::returnSession(Session* session)
{
    // Return the server.
    ///    mDs->checkinServer(session->getServer());

    // Disconnect the session.
    session->disconnect();
    
}



/**
Session* SessionIndex::getSession(string& name)
{
    StringSessionMap::iterator i = mNameSessionMap.find(name);
    if(i == mNameSessionMap.end())
        return NULL;

    return i->second;
}

Session* SessionIndex::getSession(char const* name)
{
    std::string n = name;
    return getSession(n);
}

**/

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
