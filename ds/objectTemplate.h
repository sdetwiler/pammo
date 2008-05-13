#include "object.h"
#include "db/database.h"

#include <string>
#include <map>

#ifndef PS_DS_OBJECTTEMPLATE
#define PS_DS_OBJECTTEMPLATE

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

class ObjectTemplate
{
    public:
        ObjectTemplate();
        virtual ~ObjectTemplate();

        /// Sets the database to use for this object.  This must be
        /// set before calling any methods that interact with the
        /// database.
        void setDatabase(db::Database* d);
        
        
        /// Loads the object template with the specified ObjectId
        Result load(Id id);
        
        /// Gets the name of the object type.
        std::string const* getName();

        /// Gets the ObjectId.
        Id getObjectId();

        // Creates a new object from this ObjectTemplate.
        // Does not add to the database. The ID of the returned object is invalid
        // until it has been saved to the db.
        Object* newObject();

        /// Gets a pointer to the field with the passed name.
        Field* getField(char const* name);

        /// Gets a pointer to the field with the passed name.
        Field* getField(std::string const& name);

        /// Adds a field to the ObjectTemplate.
        Result addField(Field* f);
        
        
    protected:

    private:
        friend class ObjectTemplateIndex;
        
        
        // Loads the ObjectTemplate from a ds::Table 
        Result load(Table const* table);
        

        Id mOid;
        uint32_t mCreatedTime;
        uint32_t mModifiedTime;

        // Database that contains this ObjectTemplate.
        db::Database* mDb;
        Table    mData;
};

    
///////////////////////////////////////////////////////////////
    }
}

///////////////////////////////////////////////////////////////
#endif // PS_DS_OBJECTTEMPLATE
