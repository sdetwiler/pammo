///////////////////////////////////////////////////////////////
//
// The User class.
//
// user.h
///////////////////////////////////////////////////////////////

#ifndef PS_DS_USER_H
#define PS_DS_USER_H

#include "table.h"
#include "db/database.h"
#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

class User
{
    public:
        User();
        ~User();

        void setDatabase(db::Database* d);

        Result load(Id uid);
        Result load(char const* un);

        Id getId();
        std::string const* getUsername() const;
        

        Result setUsername(char const* un);
        Result setPassword(char const* pw);
        
        
        Result checkPassword(char const* pw);
        
    protected:

        Result load(Table* t);
        
    private:
        std::string      mUsername;
        std::string      mPw;
        Id               mId;
        db::Database*    mDb;
        
};
    
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif
