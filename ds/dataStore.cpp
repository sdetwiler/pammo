#include "dataStore.h"
#include "db/server.h"
#include "db/database.h"

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

DataStore::DataStore()
{

}

DataStore::~DataStore()
{
    shutdown();
}


Result DataStore::startup(char const* hostName, char const* dbName,
                          char const* un, char const* pw,
                          int serverTypeId, size_t numConnections)
{
    mLog.open("datastore.log");

    mLog.log(Log::Medium, "DataStore startup.");
    
    mDbName = dbName;
    
    if(mCp.startup(hostName, un, pw, serverTypeId, numConnections) < 0)
    {
        mLog.log(Log::Low, "Connection pool startup failed. Is the database running?");
        return Error;
    }

    db::Server* server = mCp.checkoutServer();
    if(!server)
    {
        mLog.log(Log::Low, "Failed to checkout a server from the connection pool.");
        return Error;
    }
    
    db::Database* database = server->newDatabase();
    if(!database)
    {
        mLog.log(Log::Low, "Failed to create a database.");
        mCp.checkinServer(server);
        return Error;
    }

    database->setName(mDbName);
    
    if(mObjectTemplateIndex.startup(database) < 0)
    {
        mLog.log(Log::Low, "Failed to start the object template index.");

        delete database;
        mCp.checkinServer(server);
        return Error;
    }

    if(mSessionIndex.startup(this) < 0)
    {
        mLog.log(Log::Low, "Failed to start the session index.");
        
        delete database;
        mCp.checkinServer(server);
        return Error;
    }
    

    delete database;
    mCp.checkinServer(server);

    mLog.log(Log::Medium, "DataStore startup OK.");
    return OK;
}

void DataStore::shutdown()
{
    mLog.log(Log::Medium, "DataStore shutdown.");
    
    mSessionIndex.shutdown();
    mObjectTemplateIndex.shutdown();
    mCp.shutdown();
}



char const* DataStore::getVersion()
{
    return DATASTORE_VERSION;
}



Session* DataStore::connect(Id sid)
{
    Session* session = getSession(sid);
    if(!session)
        return NULL;

    session->connect();
    
}

Session* DataStore::connect(char const* un, char const* pw)
{
    return createSession(un, pw);
}

void DataStore::disconnect(Session* session)
{
    returnSession(session);
}

void DataStore::returnSession(Session* session)
{
    session->disconnect();
}



////////////////////////////////////////////////////////


Session* DataStore::getSession(Id sid)
{
    Session* s = mSessionIndex.getSession(sid);
    if(!s)
        return NULL;

    return s;
}

Session* DataStore::createSession(char const* un, char const* pw)
{
    return mSessionIndex.createSession(un, pw);
}

db::Server* DataStore::checkoutServer()
{
    return mCp.checkoutServer();
}


void DataStore::checkinServer(db::Server* server)
{
    mCp.checkinServer(server);
}


Result DataStore::loadObjectTemplates()
{
    return OK;
}

    
    

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
