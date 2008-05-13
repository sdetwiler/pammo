#include "session.h"
#include "db/query.h"
#include "fieldInt.h"
#include "dataStore.h"

#include <string>

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
Session::Session(DataStore* ds)
{
    mDs = ds;
    mId = 0;
    mDb = NULL;
    mServer = NULL;
}

Session::~Session()
{
    disconnect();
    
}


Result Session::connect()
{
    // If already connected.
    if(mServer)
        return OK;
    
    // Get a server.
    mServer = mDs->checkoutServer();
    if(!mServer)
        return Error;

    if(mDb)
        delete mDb;

    mDb = mServer->newDatabase();
    if(!mDb)
    {
        mDs->checkinServer(mServer);
        mServer = NULL;
        return Error;
    }
    

    mDb->setName("ds");

    return OK;
}



Result Session::disconnect()
{
    if(mDb)
    {
        delete mDb;
        mDb = NULL;
    }

    if(mServer)
    {
        mDs->checkinServer(mServer);
        mServer = NULL;
    }

    return OK;
}



Result Session::create(Id uid)
{
    if(connect() < 0)
        return Error;
    
    db::Query* query = mDb->newQuery();
    char q[128];
    sprintf(q, "INSERT INTO session VALUES(0,%d)", uid);
    if(query->execute() < 0)
    {
        delete query;
        return Error;
    }

    sprintf(q, "SELECT id FROM session WHERE uid=%d", uid);
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
    
    size_t resultCount = query->getResultCount();
    if(!resultCount)
    {
        delete query;
        return Error;
    }
    
    // Get the last session in case old sessions exist.
    Table* t = query->getResult(resultCount-1);
    if(!t)
    {
        delete query;
        return Error;
    }

    delete query;
    
    
    Field* f = t->getField("id");
    if(f == NULL)
        return Error;

    if(f->getType() != Int)
        return Error;

    mId = ((FieldInt*)f)->getIntValue();
    
    return OK;
}



Result Session::loginUser(char const* username, char const* password)
{
    if(connect() < 0)
        return Error;
    
    mUser.setDatabase(mDb);

    // Find user.
    if(mUser.load(username) < 0)
    {
        return Error;
    }

    // Validate password.
    if(mUser.checkPassword(password) < 0)
    {
        return Error;
    }

    // Create a new session.
    return create(mUser.getId());
}



Result Session::load(Id sid)
{
    if(connect() < 0)
        return Error;
    
    mId = sid;
    
    db::Query* query = mDb->newQuery();
    char q[128];
    sprintf(q, "SELECT uid FROM session WHERE id=%d", mId);
    query->setQuery(q);
    if(query->execute() < 0)
        return Error;

    if(query->saveResults() < 0)
        return Error;

    // Ensure there are results.
    size_t resultCount = query->getResultCount();
    if(!resultCount)
        return Error;

    Table* t = query->getResult(0);
    if(!t)
        return Error;

    Field* f;

    f = t->getField("uid");
    if(f == NULL)
        return Error;

    if(f->getType() != Int)
        return Error;

    // Load the matching user.
    if(mUser.load(((FieldInt*)f)->getIntValue()) < 0)
        return Error;
    
    return OK;
}


Id Session::getId()
{
    return mId;
}

User const& Session::getUser()
{
    return mUser;
}

/**
void Session::setServer(db::Server* server)
{
    mServer = server;
}
**/
db::Server* Session::getServer()
{
    return mServer;
}




db::Query* Session::newQuery()
{
    return mDb->newQuery();
}


///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
