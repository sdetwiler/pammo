#include "server.h"
#include "database.h"
///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
            namespace mysql
            {
///////////////////////////////////////////////////////////////

Server::Server()
{
    mServerHandle = NULL;
    mPort = 0;
    
}

Server::~Server()
{
    disconnect();
}

void Server::printError()
{
    if(!mServerHandle)
    {
        return;
    }
    
    printf("%s\n", mysql_error(mServerHandle));
}

Result Server::connect()
{
    if(mServerHandle)
    {
        return Error;
    }

    mServerHandle = mysql_init(NULL);
    if(mServerHandle == NULL)
    {
        return Error;
    }
    
    unsigned long flags=0;
    if(mysql_real_connect(mServerHandle,
                          getHost()->c_str(),
                          getUsername()->c_str(),
                          getPassword()->c_str(),
                          NULL,
                          mPort,
                          NULL,
                          flags) == NULL)
    {

        printError();
        
        return Error;
    }
    
    return OK;
}

    
Result Server::disconnect()
{
    if(mServerHandle)
    {
        mysql_close(mServerHandle);
        mServerHandle = NULL;
    }

    return OK;
}

MYSQL* Server::getHandle() const
{
    return mServerHandle;
}

db::Database* Server::newDatabase()
{
    return new Database(this);
}


///////////////////////////////////////////////////////////////
            }
        }
    }
}
///////////////////////////////////////////////////////////////
