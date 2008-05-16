#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define PAMMO_OK                        0
#define PAMMO_ERROR_UNKNOWNMAP         -1
#define PAMMO_ERROR_ALREADYLOGGEDIN    -2

// Uniquely identifies an instance of a map on a server.
class MapInstanceId
{
    public:
        
        uint16_t mServerId;   // Unique id of the server.
        uint16_t mMapId;      // Unique id of the map.
        uint32_t mInstanceId; // Instance cound of map on the identified server.

        MapInstanceId()
        {
            mServerId = 0;
            mMapId = 0;
            mInstanceId = 0;
        }
        
        MapInstanceId const& operator=(MapInstanceId const& rhs)
        {
            *((uint64_t*)this) = *((uint64_t*)(&rhs));

            return *this;
        }

        bool operator==(MapInstanceId const& rhs) const
        {
            if(*((uint64_t*)this) == *((uint64_t*)(&rhs)) )
                return true;
            return false;
        }
        bool operator<(MapInstanceId const& rhs) const
        {
            if(*((uint64_t*)this) < *((uint64_t*)(&rhs)))
                return true;
            return false;
        }
        bool operator>(MapInstanceId const& rhs) const
        {
            if(*((uint64_t*)this) > *((uint64_t*)(&rhs)))
                return true;
            return false;
        }
};



void hexdump(char const* str, void* buf, uint32_t bufLen);



#endif // TYPES_H
