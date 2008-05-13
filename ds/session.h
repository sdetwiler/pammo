///////////////////////////////////////////////////////////////
//
// The Session class.
//
// session.h
///////////////////////////////////////////////////////////////
#ifndef PS_DS_SESSION_H
#define PS_DS_SESSION_H

#include "user.h"
#include "db/database.h"
#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
class DataStore;
    
class Session
{
    public:
        Session(DataStore* ds);
        ~Session();


        // Connects the session to the DataStore.
        Result connect();

        // Disconnects the session from the DataStore.
        Result disconnect();
        

        Result loginUser(char const* username, char const* password);
        
        // Creates a new session for the passed user Id. Generates a
        // new unique session Id and saves in database.
        Result create(Id uid);
        
        
        // Loads a session for the passed Id.
        Result load(Id sid);

        // Gets the session Id.
        Id getId();

        // Gets the user associated with this session.
        User const& getUser();


        // Query the DataStore.
        db::Query* newQuery();

        
        
    protected:

    private:
        friend class SessionIndex;
        
        //        void setDatabase(db::Database* d);
        //        void setServer(db::Server* server);
        db::Server* getServer();
        

        
        DataStore*  mDs;      // DataStore pointer.
        
        db::Server* mServer;  // DataStore database server.
        
        db::Database* mDb;    // User database.
        
        User        mUser;
        Id          mId;
};
    
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif
