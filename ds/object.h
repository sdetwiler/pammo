///////////////////////////////////////////////////////////////
// object.h
///////////////////////////////////////////////////////////////
#ifndef PS_DS_OBJECT_H
#define PS_DS_OBJECT_H

#include "types.h"
#include "table.h"
#include "db/database.h"

#include <string>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////
class Object
{
    public:
        Object(db::Database* db);
        virtual ~Object();

        // Copies object. This object becomes a new instance and is NOT stored in the database.
        // The ID of this object is not valid until saved.
        Object& operator=(Object const& o);
        
        /// Loads the object with the specified Id.
        Result load(Id id);
        
        /// Saves the object to the database. Adds if this is a new object.
        Result save();

        /// Gets the name of this object type.
        std::string const* getName();

        /// Returns the unique Id for this object.
        Id getId();
        
        /// Returns the Id of the object type.
        Id getObjectId();
        
        /// Gets a pointer to the field with the passed name.
        Field* getField(char const* name);

        /// Gets a pointer to the field with the passed name.
        Field* getField(std::string const& name);
        
        
    protected:
        
    private:
        friend class ObjectTemplate;

        // Adds object to database.
        Result add();
        
        Id            mId;           // Id of this object. Not set until stored in DB.
        Id            mOid;          // ObjectId for this object.
        uint32_t      mCreatedTime;  // When was this object created.
        uint32_t      mModifiedTime; // When was this object last updated.
        db::Database* mDb;           // Database pointer for storage.
        Table         mData;         // Internal storage for fields (Dynamic DataStore).

        enum DbState
        {
            Fresh = 0,               // DB and Object are equal.
            NotStored = -1,          // No copy in DB.
            Stale = -2,              // Old copy in DB.
        };
        
        DbState       mDbState;      // State of this object in the database.
};
    


///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
#endif
