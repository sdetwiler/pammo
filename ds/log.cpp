#include "log.h"

#include <time.h>
#include <stdarg.h>

using namespace std;

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
///////////////////////////////////////////////////////////////

Log::Log()
{
    mF = NULL;
    pthread_mutex_init(&mLock, NULL);
}


Log::~Log()
{
    close();
    pthread_mutex_destroy(&mLock);
}


Result Log::open(char const* filename, Level l)
{
    close();

    setLevel(l);
    mF = fopen(filename, "w");
}


void Log::close()
{
    if(mF)
    {
        fclose(mF);
        mF = NULL;
    }
}


void Log::setLevel(Level l)
{
    mLevel = l;
}


void Log::log(Level l, char const* fmt, ...)
{
    if(l < mLevel)
        return;

    if(!mF)
        return;
    
    char buf[512];
    size_t len;
    
    
    
    va_list ap;
    va_start(ap, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    char msg[len+32];
    
    time_t now = time(NULL);

    char t[25];
    ctime_r(&now, t);
    t[24]=0;
    
    pthread_mutex_lock(&mLock);

    sprintf(msg, "%s\t%s\n", t, buf);
    fwrite(msg, strlen(msg), 1, mF);
    
    
    pthread_mutex_unlock(&mLock);
    
}
    
    

///////////////////////////////////////////////////////////////
    }
}
///////////////////////////////////////////////////////////////
