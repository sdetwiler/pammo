///////////////////////////////////////////////////////////////
// query.h
//
// Query base class.
//
//
///////////////////////////////////////////////////////////////

#ifndef PS_DS_DB_QUERY_H
#define PS_DS_DB_QUERY_H

#include "../types.h"
#include "../table.h"
//#include "database.h"
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
        
class Query
{
    public:
        
        Query(Database* db);
        virtual ~Query();

        void setQuery(std::string const& n);
        void setQuery(char const* n);
        std::string const* getQuery() const;
       
        void setDatabase(Database* db);
        Database* getDatabase() const;

        virtual Result execute()=0;
        virtual Result saveResults()=0;

        // Get vector of results.
        TableVec* getResults();

        // Get n'th result. Zero based.
        Table* getResult(size_t n);

        // Get number of results.
        size_t getResultCount();
        
        virtual void clear();
        
        
    protected:
        TableVec     mResults;
        /// Add Table to the results.
        virtual Result addTable(Table* table);
        
        
    private:
        Database* mDatabase;
        std::string mQuery;
        
};

///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_DB_QUERY_H
