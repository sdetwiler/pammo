///////////////////////////////////////////////////////////////
// query.h
//
// MySQL Query class.
//
//
///////////////////////////////////////////////////////////////
#ifndef PS_DS_DB_MYSQL_QUERY_H
#define PS_DS_DB_MYSQL_QUERY_H

#include "../query.h"


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
            
class Query : public db::Query
{
    public:
        Query(mysql::Database* database);
        virtual ~Query();

        virtual Result execute();
        virtual Result saveResults();
        
    protected:

    private:
        Type normalizeFieldType(int type);
};
            
///////////////////////////////////////////////////////////////
            }
        }
    }
}
///////////////////////////////////////////////////////////////      

#endif // PS_DS_DB_MYSQL_QUERY_H
