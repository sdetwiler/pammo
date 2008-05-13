///////////////////////////////////////////////////////////////
// The ObjectTemplateIndex class indexes all ObjectTemlates,
// which are used for object creation.
///////////////////////////////////////////////////////////////

#include "objectTemplate.h"
#include "db/database.h"

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

#ifndef PS_DS_OBJECTTEMPLATEINDEX
#define PS_DS_OBJECTTEMPLATEINDEX


class ObjectTemplateIndex
{
    public:
        ObjectTemplateIndex();
        ~ObjectTemplateIndex();

        Result startup(db::Database* database);
        void shutdown();
        
        /// Adds an ObjectTemplate to the map. This index will dealloc ObjectTemplate.
        Result addObjectTemplate(ObjectTemplate* objTemp);

        ObjectTemplate* getObjectTemplate(Id oid);
        ObjectTemplate* getObjectTemplate(std::string& name);
        ObjectTemplate* getObjectTemplate(char const* name);
        
    protected:

    private:
        typedef std::map<Id, ObjectTemplate*> IdObjectTemplateMap;
        typedef std::map<std::string, ObjectTemplate*>StringObjectTemplateMap;

        IdObjectTemplateMap     mIdObjectTemplateMap;
        StringObjectTemplateMap mNameObjectTemplateMap;
        
};
    
///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_OBJECTTEMPLATEINDEX
