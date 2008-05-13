#include "fieldString.h"
#include "fieldInt.h"

#include "table.h"


#include <iostream>
#include <string>

#include "db/query.h"

#include "db/mysql/server.h"
#include "db/mysql/database.h"

#include "db/mysql/query.h"

using namespace std;
using namespace ps::ds;

using namespace db::mysql;



void testXmlElement()
{

    XmlElement xe;
    string xml;
    
    xe.setName("foo");
    xe.setValue("bar");
    xe.getXml(xml);
    if( xml.compare("<foo>bar</foo>") != 0)
    {
        cout << "Setting failed. getXml returned:\n" << xml.c_str() << endl;
        exit(1);
    }

    xe.clear();

    if(xe.setXml(xml) < 0)
    {
        cout << "parse failed\n";
        exit(1);
    }

    xe.getXml(xml);
    if(xml.compare("<foo>bar</foo>") != 0)
    {
        cout << "Parse failed. getXml returned:\n" << xml.c_str() << endl;
        exit(1);
    }

    xe.clear();

    xml = "<foo key=\"value\"><bar fortytwo=\"42\"></bar></foo>";
    if(xe.setXml(xml) < 0)
    {
        cout << "setXml failed.\n";
        exit(1);
    }

    cout << "numChildren: " << xe.getNumChildren() << endl;
    

    string cmpXml;
    xe.getXml(cmpXml);
    if(xml != cmpXml)
    {
        cout << "compare failed\n" << cmpXml.c_str();
        exit(1);
    }
    
    
    
        
}




void testTableSaveLoad(Table* t)
{
    string xml;

    t->getAsXml(xml);
    cout << "Table::getAsXml:\n" << xml.c_str() << endl;

    t->clear();

    if(t->loadFromXml(xml) < 0)
    {
        cerr << "Table::loadFromXml failed\n";
        exit(1);
    }

    t->getAsXml(xml);

    cout << "Table::getAsXml:\n" << xml.c_str() << endl;
}


void testFieldSaveLoad(Field* f)
{
    string xml;
    
    f->getAsXml(xml);

    cout << "getAsXml:\n" << xml.c_str() << endl;


    f->clear();

    if(f->loadFromXml(xml) < 0)
    {
        cerr<< "loadFromXml failed\n";
        exit(1);
    }

    f->getAsXml(xml);

    cout << "getAsXml:\n" << xml.c_str() << endl;
}

void testFieldNameChange(Field* f, char const* n)
{
    string const* name = f->getName();
    if(!name)
    {
        cout << "name is NULL\n";
    }
    else
        cout << name->c_str() << endl;

    cout << "Changing name to " << n << endl;
    
    f->setName(n);

    name = f->getName();
    if(!name)
    {
        cout << "name is NULL\n";
        exit(1);
    }
    else
        cout << name->c_str() << endl;

}


void testConnectMySql()
{
    Result res;
 
    cout << "Testing MySQL Server Driver\n";
    
    Server server;
    server.setHost("localhost");
    server.setUsername("ds");    // The ds user.
    server.setPassword("ds");    // The secret ds password.
    cout << "Now connecting...\n";
    
    res = server.connect();
    if(res < 0)
    {
        cout << "server connect failed\n";
    }
    else
    {
        cout << "server connect succeeded\n";
    }

    Database database(&server);
    database.setName("ds");
    res = database.connect();
    if(res < 0)
    {
        cout << "database connect failed.\n";
    }
    else
    {
        cout << "database connect succeeded\n";
    }



    Query query(&database);


    query.setQuery("INSERT INTO object VALUES (0,1,'2005-10-26', '2005-10-26', 'This is test data.');");
    res = query.execute();
    res = query.execute();
    if(res < 0)
    {
        cout << "insert query failed\n";
    }


    
    query.setQuery("SELECT * FROM object");
    res = query.execute();
    if(res < 0)
    {
        cout << "query failed.\n";
    }
    else
    {
        cout << "query succeeded\n";
    }

    res = query.saveResults();
    if(res < 0)
    {
        cout << "saveResults failed.\n";
    }
    else
    {
        cout << "saveResults succeeded\n";
    }



    string xml;
    
    TableVec* results = query.getResults();
    TableVec::iterator i = results->begin();
    while(i != results->end())
    {
        res = (*i)->getAsXml(xml);
        if(res < 0)
        {
            cout << "Failed to get results.\n";
        }

        cout << "Result\n------\n";
        cout << xml.c_str() << endl;
        
        i++;
    }

    cout << endl;
    


    

    query.setQuery("DELETE FROM object WHERE ID>0");
    res = query.execute();
    if(res < 0)
    {
        cout << "delete query failed\n";
    }
    else
    {
        cout << "delete query succeeded\n";
    }
    
    

    res = database.disconnect();
    if(res < 0)
    {
        cout << "database disconnect failed.\n";
    }
    
    else
    {
        cout << "database disconnect succeeded\n";
    }

    
    

    res = server.disconnect();
    if(res < 0)
    {
        cout << "server disconnect failed\n";
    }
    else
    {
        cout << "server disconnect succeeded\n";
    }
}

int main(int argc, char** argv)
{

    testXmlElement();

    //    return 0;
    
    testConnectMySql();
    

    FieldString* fStr = new FieldString;

    fStr->setName("string");
    fStr->setValue("Test Value");
    fStr->setAttribute("foo", "bar");

    testFieldSaveLoad(fStr);
    testFieldNameChange(fStr, "string2");
    

    // Test Assignment operator
    FieldString* fStrB = new FieldString;

    *fStrB = *fStr;
    

    
    FieldInt* fInt = new FieldInt;
    fInt->setName("integer");
    fInt->setValue("42");
    fInt->setAttribute("foo", "bar");

    testFieldSaveLoad(fInt);
    testFieldNameChange(fInt, "integer2");



    Table table;
    table.setName("aTable");
    table.setAttribute("key", "value");

    testTableSaveLoad(&table);


    table.addField(fStr);
    table.addField(fInt);

    testTableSaveLoad(&table);


    Table table2;

    // Test table assignment operator.
    table2 = table;

    // show results.
    testTableSaveLoad(&table2);

    // Test file serialization
    table.saveToFile("test.xml");
    table.clear();
    table.loadFromFile("test.xml");

    testTableSaveLoad(&table);
    
    

    //delete fStr;
    //delete fInt;
    
    
    
    return 0;
}

    
    
    
