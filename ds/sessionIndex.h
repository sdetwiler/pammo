///////////////////////////////////////////////////////////////
// The SessionIndex class indexes all Sessions, which are used to
// track current users.
//
// TODO: The session maps should flush LRU Sessions to save mem.
//
//
///////////////////////////////////////////////////////////////

#include "session.h"
#include "db/database.h"

#ifndef PS_DS_SESSIONINDEX
#define PS_DS_SESSIONINDEX

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////


class DataStore;
    

class SessionIndex
{
    public:
        SessionIndex();
        ~SessionIndex();

        Result startup(DataStore* datastore);
        void shutdown();
        

        /// Creates a session for the specified user if the un/pw
        /// combo match.  Returns new session or NULL on error. Must
        /// return session after use.
        Session* createSession(char const* username, char const* password);

        // Gets the session associated with the passed session
        // id. Must return session after use.
        Session* getSession(Id sid);


        void returnSession(Session* session);
        

        //   Session* getSession(std::string& username);
        //   Session* getSession(char const* username);
        
    protected:

    private:


        ps::ds::DataStore* mDs;
        

        typedef std::map<Id, Session*> IdSessionMap;
        typedef std::map<std::string, Session*>StringSessionMap;

        IdSessionMap     mIdSessionMap;
        StringSessionMap mNameSessionMap;
        
};
    
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_SESSIONINDEX
