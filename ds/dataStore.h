//////////////////////////////////////////////////////////////////////
// The DataStore class provides the primary interface to the DataStore
// functionality.
//////////////////////////////////////////////////////////////////////

#include "db/connectionPool.h"
#include "objectTemplateIndex.h"
#include "sessionIndex.h"
#include "log.h"

#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

#ifndef PS_DS_DATASTORE
#define PS_DS_DATASTORE

#define DATASTORE_VERSION "0.1"
    
class DataStore
{
    public:
        DataStore();
        virtual ~DataStore();

        Result startup(char const* hostName, char const* dbName,
                       char const* un, char const* pw,
                       int serverTypeId, size_t numConnections=10);

        void shutdown();


        // Connect to the DataStore. Pass a session id or zero for the guest session.
        Session* connect(Id sid=0);
        Session* connect(char const* un, char const* pw);
        
        
        // Disconnect from the DataStore. Must be match call to connect.
        void disconnect(Session* session);

        char const* getVersion();
        

    protected:

    private:
        friend class SessionIndex;
        friend class Session;
        
        db::Server* checkoutServer();
        void checkinServer(db::Server* server);
        
        // Gets the session for the specified session id.
        Session* getSession(Id sid);
        // Return the session after use.
        void returnSession(Session* session);
        

        // Creates a session for the user. Verifies password. Returns
        // NULL on failure.
        Session* createSession(char const* un, char const* pw);


        
        Result loadObjectTemplates();
        
        std::string           mDbName;
        db::ConnectionPool    mCp;
        ObjectTemplateIndex   mObjectTemplateIndex;
        SessionIndex          mSessionIndex;
        Log                   mLog;
};

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_DATASTORE
