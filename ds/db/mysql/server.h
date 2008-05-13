///////////////////////////////////////////////////////////////
// serverMySql.h
//
// MySQL Server class.
//
//
///////////////////////////////////////////////////////////////
#ifndef PS_DS_DB_MYSQL_SERVER_H
#define PS_DS_DB_MYSQL_SERVER_H

#include "../server.h"

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
class Database;
            
class Server : public db::Server
{
    public:
        Server();
        virtual ~Server();

        virtual Result connect();
        virtual Result disconnect();

        virtual db::Database* newDatabase();
        
        
        MYSQL* getHandle() const;
    protected:
 
        
    private:
        MYSQL* mServerHandle;
        unsigned int mPort;

        void printError();
};

        
///////////////////////////////////////////////////////////////
            }
        }
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_SERVERMYSQL_H
