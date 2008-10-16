#include "pammo.h"
#include <pthread.h>

namespace pammo
{

void spawnThread(void* (*threadFunc)(void*), void* arg)
{
    pthread_t thread;
    pthread_create(&thread, NULL, threadFunc, arg);  
}

}