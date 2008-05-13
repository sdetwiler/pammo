//////////////////////////////////////////////////////////////////////
// The Log class provides logging support
//////////////////////////////////////////////////////////////////////

#include "types.h"

#include <string>
#include <stdio.h>
#include <pthread.h>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

#ifndef PS_DS_LOG
#define PS_DS_LOG

    
class Log
{
    public:

        enum Level    // Levels of logging to report. Set log priority as inverse.
        {
            Debug=0,
            High=1,
            Medium=2,
            Low=3
        };
        
        
        Log();
        virtual ~Log();

        void setLevel(Level l);
        
        Result open(char const* filename, Level l=Debug);
        void close();

        void log(Level l, char const* fmt, ...);
        
    protected:

    private:
        Level mLevel;
        FILE* mF;

        pthread_mutex_t mLock;
};

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////

#endif // PS_DS_DATASTORE
