#include "serverFactory.h"

#include <queue>
#include <pthread.h>

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////

class ConnectionPool
{
    public:
        ConnectionPool();
        virtual ~ConnectionPool();

        Result startup(char const* hostName, char const* un, char const* pw, int serverTypeId, size_t numConnections=10);
        void shutdown();

        /// Checkout a server from the ConnectionPool.
        Server* checkoutServer();

        /// Checkin a server to the ConnectionPool.
        void checkinServer(Server* server);
        
        
        
    protected:
        
    private:
        typedef std::queue<Server*> ServerQueue;

        ServerQueue mServerQueue;

        pthread_mutex_t mLock;
        
};
        
///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////
