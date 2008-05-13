///////////////////////////////////////////////////////////////
// database.h
//
// MySQL Database class.
//
//
///////////////////////////////////////////////////////////////
#ifndef PS_DS_DB_MYSQL_DATABASE_H
#define PS_DS_DB_MYSQL_DATABASE_H

#include "server.h"
#include "../database.h"

#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>
#include <mysql.h>
#include <mysql_embed.h>
#include <errmsg.h>
#include <my_getopt.h>

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
            //class Query;
            
class Database : public db::Database
{
    public:
        Database(mysql::Server const* server);
        virtual ~Database();

        virtual Result connect();
        virtual Result disconnect();

        virtual db::Query* newQuery();
        
    protected:

    private:
};
///////////////////////////////////////////////////////////////
            }
        }
    }
}
///////////////////////////////////////////////////////////////

        

#endif // PS_DS_DB_MYSQL_DATABASE_H
