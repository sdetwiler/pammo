#include <iostream>
#include <string>

#include "../db/connectionPool.h"
#include "../db/server.h"
#include "../db/database.h"
#include "../db/query.h"

using namespace std;

using namespace ps;
using namespace ds;
using namespace db;


int main(int argc, char** argv)
{
    // Create a ConnectionPool. This maintains a set of connections to the specified server.
    ConnectionPool cp;
    if(cp.startup("localhost", "ds", "ds", PS_DS_DB_SERVER_MYSQL, 5) < 0)
    {
        cout << "Failed to start ConnectionPool\n";
        return 1;
    }

    // Check out a server connection.
    Server* server = cp.checkoutServer();
    if(!server)
    {
        cout << "Failed to checkout a server.\n";
        return 1;
    }


    // Create a database object for the returned server.
    Database* database = server->newDatabase();
    database->setName("ds");
    if(database->connect() < 0)
    {
        cout << "Failed to connect to database.\n";
        return 1;
        
    }

    // Create a query object for the returned database.
    Query* query = database->newQuery();
    query->setQuery("INSERT INTO object VALUES (0,1,'2005-10-26', '2005-10-26', 'This is test data.');");
    if(query->execute() < 0)
    {
        cout << "Failed to execute INSERT query.\n";
        return 1;
    }

    // Reuse the query object.
    query->setQuery("SELECT data FROM object");
    if(query->execute() < 0)
    {
        cout << "Failed to execute SELECT query.\n";
        return 1;
    }

    // Save the results of the query.
    if(query->saveResults() < 0)
    {
        cout << "Failed to save query results.\n";
    }

    
    string xml;

    // Retrieve the results as xml.
    TableVec* results = query->getResults();
    TableVec::iterator i = results->begin();
    while(i != results->end())
    {
        if((*i)->getAsXml(xml) < 0)
        {
            cout << "Failed to get results.\n";
        }

        cout << "Result\n------\n";
        cout << xml.c_str() << endl;
        
        i++;
    }

    cout << endl;

    // Reuse the query again to delete what was added.
    query->setQuery("DELETE FROM object WHERE ID>0");
    if(query->execute() < 0)
    {
        cout << "Failed to execute DELETE query.\n";
        return 1;
    }

    // Delete the objects.
    delete query;
    delete database;

    // Checkin the server.
    cp.checkinServer(server);
    
    
    

    
    return 0;
}
