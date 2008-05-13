#include "server.h"
#include "database.h"
#include "query.h"

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


Database::Database(mysql::Server const* server)
    :db::Database(server)
{
}

Database::~Database()
{}


Result Database::connect()
{
    Server const* server = (Server const*) getServer();
    if(!server)
        return Error;

    if(!mysql_select_db(server->getHandle(), getName()->c_str()))
        return OK;

    return Error;
}


    
    
Result Database::disconnect()
{
    return OK;
}

db::Query* Database::newQuery()
{
    return new Query(this);
}

///////////////////////////////////////////////////////////////
            }
        }
    }
}
///////////////////////////////////////////////////////////////
