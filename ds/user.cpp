///////////////////////////////////////////////////////////////
//
// The User class.
//
// user.cpp
///////////////////////////////////////////////////////////////

#include "user.h"
#include "db/query.h"
#include "fieldInt.h"

#include <string>

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
  

User::User()
{
    mId = 0;
    mDb = NULL;
}


User::~User()
{
    //
}


Result User::load(Id uid)
{
    if(!mDb)
        return Error;
    
    db::Query* query = mDb->newQuery();
    char q[128];
    snprintf(q, sizeof(q), "SELECT * FROM user WHERE id=%d", uid);
    query->setQuery(q);
    if(query->execute() < 0)
        return Error;

    if(query->saveResults() < 0)
        return Error;

    // Ensure there are results.
    size_t resultCount = query->getResultCount();
    if(!resultCount)
        return Error;

    Table* t = query->getResult(0);
    if(!t)
        return Error;

    return load(t);
}


Result User::load(char const* un)
{
    if(!mDb)
        return Error;
    
    db::Query* query = mDb->newQuery();
    char q[128];
    snprintf(q, sizeof(q), "SELECT * FROM user WHERE username=\"%s\"", un);
    query->setQuery(q);
    if(query->execute() < 0)
        return Error;

    if(query->saveResults() < 0)
        return Error;

    // Ensure there are results.
    size_t resultCount = query->getResultCount();
    if(!resultCount)
        return Error;

    Table* t = query->getResult(0);
    if(!t)
        return Error;

    return load(t);
}


Result User::load(Table* t)
{
    Field* f;

    f = t->getField("id");
    if(!f)
        return Error;
    if(f->getType() != Int)
        return Error;

    mId = ((FieldInt*)f)->getIntValue();
    
    f = t->getField("username");
    if(!f)
        return Error;

    f->getStringEncValue(mUsername);
    
    f = t->getField("password");
    if(!f)
        return Error;

    f->getStringEncValue(mPw);
    
    return OK;
}


void User::setDatabase(db::Database* d)
{
    mDb = d;
}



Id User::getId()
{
    return mId;
}

string const* User::getUsername() const
{
    return &mUsername;
}

Result User::setUsername(char const* un)
{
    mUsername = un;
    return OK;
}

Result User::setPassword(char const* pw)
{
    mPw = pw;
    return OK;
}


Result User::checkPassword(char const* pw)
{
    // TODO: hash and compare.
    return OK;
}

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
