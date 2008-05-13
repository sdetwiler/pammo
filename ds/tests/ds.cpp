#include "../dataStore.h"
#include "../db/query.h"

#include <iostream>
#include <string>

using namespace std;

using namespace ps;
using namespace ds;
using namespace db;


int main(int argc, char** argv)
{
    // Create a DataStore.
    cout << "a\n";
    
    DataStore dataStore;
    if(dataStore.startup("localhost", "ds", "ds", "ds",  PS_DS_DB_SERVER_MYSQL, 5) < 0)
    {
        cerr << "DataStore.startup failed.\n";
        return 1;
    }

    cout << "b\n";

    // Connect as the guest user.
    Session* session = dataStore.connect(0);
    if(!session)
    {
        cerr << "DataStore.connect(0) failed\n";
        return 1;
    }

    cout << "c\n";

    // Get a query object.
    Query* query = session->newQuery();
    if(!query)
    {
        cerr << "Session.newQuery failed\n";
        return 1;
    }

    cout << "d\n";

    query->setQuery("INSERT INTO object VALUES (0,1,'2005-10-26', '2005-10-26', 'This is test data.');");
    if(query->execute() < 0)
    {
        cout << "Failed to execute INSERT query.\n";
        return 1;
    }

    cout << "e\n";
    
    // Reuse the query object.
    query->setQuery("SELECT data FROM object");
    if(query->execute() < 0)
    {
        cout << "Failed to execute SELECT query.\n";
        return 1;
    }

    cout << "f\n";

    // Save the results of the query.
    if(query->saveResults() < 0)
    {
        cout << "Failed to save query results.\n";
    }

    cout << "g\n";
    
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
    cout << "h\n";

    // Reuse the query again to delete what was added.
    query->setQuery("DELETE FROM object WHERE ID>0");
    if(query->execute() < 0)
    {
        cout << "Failed to execute DELETE query.\n";
        return 1;
    }

    cout << "i\n";

    // Delete the objects.
    delete query;
    
    cout << "j\n";

    // Disconnect the session.
    dataStore.disconnect(session);

    cout << "k\n";

    dataStore.shutdown();
    
    cout << "l\n";
    

    return 0;
}
