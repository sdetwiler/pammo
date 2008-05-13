///////////////////////////////////////////////////////////////
// server.h
//
// Server base class.
//
//
///////////////////////////////////////////////////////////////
#ifndef PS_DS_DB_SERVER_H
#define PS_DS_DB_SERVER_H

#include "../types.h"

#include <string>
///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////
class Database;
        
class Server
{
    public:
        Server();
        virtual ~Server();


        void setHost(std::string const& host);
        void setHost(char const* host);
        std::string const* getHost() const;

        void setUsername(std::string const& un);
        void setUsername(char const* un);
        std::string const* getUsername() const;

        void setPassword(std::string const& pw);
        void setPassword(char const* pw);
        std::string const* getPassword() const;
        
        virtual Result connect()=0;

        virtual Result disconnect()=0;
        /// Allocates a new Database object for this database.
        /// Caller is responsible for deallocation.
        virtual Database* newDatabase()=0;
        
        
    protected:

    private:

        std::string mHost;
        std::string mUsername;
        std::string mPassword;
        
};

///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_DB_SERVER_H
