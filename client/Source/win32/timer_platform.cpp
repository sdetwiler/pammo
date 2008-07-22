#include "platform.h"
namespace pammo
{
uint64_t getTime()//Timer* timer)
{
    QueryPerformanceCounter(&gTimer.now);
    return gTimer.startTime + ((uint64_t)gTimer.now.QuadPart * 1000000) / (uint64_t)gTimer.freq.QuadPart;
}


} // namespace pammo