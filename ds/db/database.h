///////////////////////////////////////////////////////////////
// database.h
//
// Database base class.
//
//
///////////////////////////////////////////////////////////////

#ifndef PS_DS_DB_DATABASE_H
#define PS_DS_DB_DATABASE_H

#include "../types.h"
#include "server.h"
#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////
class Query;

class Database
{
    public:
        Database(Server const* server);
        virtual ~Database();

        void setName(std::string const& n);
        void setName(char const* n);
        std::string const* getName() const;
       
        void setServer(Server const* server);
        Server const* getServer() const;

        virtual Result connect() = 0;
        virtual Result disconnect() = 0;

        virtual Query* newQuery();
        
    protected:

    private:
        Server const* mServer;
        std::string mName;
        
};

///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_DB_DATABASE_H
