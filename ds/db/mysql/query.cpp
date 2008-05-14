#include "query.h"
#include "server.h"
#include "database.h"
#include "../../fieldFactory.h"

#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>
#include <mysql.h>
#include <mysql_embed.h>
#include <errmsg.h>
#include <my_getopt.h>

using namespace std;

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


Query::Query(mysql::Database* db)
    :db::Query(db)
{
}

Query::~Query()
{
}

    

Result Query::execute()
{
    mysql::Database* database = (mysql::Database*)getDatabase();
    if(!database)
        return Error;
    
    // Ensure the correct database is current.
    database->connect();

    mysql::Server* server = (mysql::Server*)database->getServer();

    std::string const* q = getQuery();

    int res =mysql_query(server->getHandle(), q->c_str());
    
    if(!res)
        return OK;

    printf("Query::execute error %d\n", res);
    
    return Error;
}

Type Query::normalizeFieldType(int type)
{
    switch(type)
    {
    case 3:          // int(4)
        return Int;
    case 12:         // datetime
    case 252:        // text
    default:
        return String;
    }
}

Result Query::saveResults()
{
    mysql::Database* database = (mysql::Database*)getDatabase();
    if(!database)
        return Error;

    mysql::Server const* server = (mysql::Server*)database->getServer();
    MYSQL* handle = server->getHandle();
    
    MYSQL_RES* result = mysql_store_result(handle);
    if(!result)
        return Error;
    
    unsigned int numFields = mysql_num_fields(result);
    MYSQL_ROW row;
    MYSQL_FIELD* msField;
    
    Table tableTmpl;
    Table* table;
    Field* field;
    Type fieldType;
    int i;

    // First, build the table to store the results.
    // For each field in the row.
    for(i=0; i<numFields; ++i)
    {
        msField = mysql_fetch_field(result);
        if(!msField)
        {
            return Error;
        }
        
        fieldType = normalizeFieldType(msField->type);
        if(fieldType == Unknown)
        {
            continue;
        }
            
        field = FieldFactory::create(fieldType);
        if(!field)
        {
            mysql_free_result(result);
            return Error;
        }
        
        field->setName(msField->name);
        tableTmpl.addField(field);
    }

    // Reset cursor.
    mysql_field_seek(result, 0);
    //    mysql_row_seek(result, 0);
    
    
    // For every row returned.
    while((row = mysql_fetch_row(result)))
    {
        // Create a new table.
        table = new Table;
        // Copy the table template.
        *table = tableTmpl;

        // For each field in the row.
        for(i=0; i<numFields; ++i)
        {
            field = table->getField(i);
            if(!field)
            {
                printf("failed to get field %d\n", i);

                clear(); // Clear any loaded results.
                delete table;
                mysql_free_result(result);
                
                return Error;
            }
            
            field->setValue(row[i]);
        }

        // Add table to the results.
        addTable(table);
    }
    
    mysql_free_result(result);
    
    return OK;
}

    

///////////////////////////////////////////////////////////////
            }
        }
    }
}
///////////////////////////////////////////////////////////////
