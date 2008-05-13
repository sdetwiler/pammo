#include "query.h"
///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////


Query::Query(Database* db)
{
    mDatabase = db;
}


Query::~Query()
{
    clear();
}

void Query::clear()
{
    for(TableVec::iterator i = mResults.begin(); i != mResults.end(); i++)
    {
        delete *i;
    }
    
     mResults.clear();
}



void Query::setQuery(std::string const& query)
{
    mQuery = query;
}


void Query::setQuery(char const* query)
{
    mQuery = query;
}

std::string const* Query::getQuery() const
{
    return &mQuery;
}

Database* Query::getDatabase() const
{
    return mDatabase;
}


TableVec* Query::getResults()
{
    return &mResults;
}

Table* Query::getResult(size_t n)
{
    if(mResults.size() < n)
        return NULL;
    
    return mResults[n];
}

size_t Query::getResultCount()
{
    return mResults.size();
}


Result Query::addTable(Table* table)
{
    mResults.push_back(table);
    return OK;
}

///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////
