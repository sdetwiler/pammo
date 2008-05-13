#include "database.h"

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////


Database::Database(Server const* server)
{
    mServer = server;
}


Database::~Database()
{}


void Database::setName(std::string const& n)
{
    mName = n;
}


void Database::setName(char const* n)
{
    mName = n;
}

std::string const* Database::getName() const
{
    return &mName;
}

Server const* Database::getServer() const
{
    return mServer;
}

Query* Database::newQuery()
{
    return NULL;
}

///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////
