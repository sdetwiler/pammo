#include "connectionPool.h"
#include "serverFactory.h"

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////

ConnectionPool::ConnectionPool()
{
    pthread_mutex_init(&mLock, NULL);
}

ConnectionPool::~ConnectionPool()
{
    shutdown();

    pthread_mutex_destroy(&mLock);
}

void ConnectionPool::shutdown()
{
    Server* server;

    pthread_mutex_lock(&mLock);
    
    while(mServerQueue.empty() == false)
    {
        
        server = mServerQueue.front();
        delete server;
        mServerQueue.pop();
    }

    pthread_mutex_unlock(&mLock);
}

    
        
Result ConnectionPool::startup(char const* hostName, char const* un,
                               char const* pw, int serverTypeId,
                               size_t numConnections)
{
    pthread_mutex_lock(&mLock);

    for(size_t i = 0; i < numConnections; ++i)
    {
        Server* server = ServerFactory::create(serverTypeId);
        if(!server)
        {
            pthread_mutex_unlock(&mLock);
            return Error;
        }
        
        server->setHost(hostName);
        server->setUsername(un);
        server->setPassword(pw);
        if(server->connect() < 0)
        {
            delete server;
            pthread_mutex_unlock(&mLock);
            return Error;
        }

        mServerQueue.push(server);
    }

    pthread_mutex_unlock(&mLock);

    return OK;
}

    
Server* ConnectionPool::checkoutServer()
{
    Server* server;
    pthread_mutex_lock(&mLock);
    
    if(mServerQueue.empty())
    {
        server = NULL;
    }
    
    else
    {
        server = mServerQueue.front();
        mServerQueue.pop();
    }
    
    pthread_mutex_unlock(&mLock);

    return server;
}

void ConnectionPool::checkinServer(Server* server)
{
    pthread_mutex_lock(&mLock);

    mServerQueue.push(server);

    pthread_mutex_unlock(&mLock);
}



///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////
