#include "server.h"

#include "serverSession.h"
#include "serverMapIndex.h"

class GameServer
    : public ServerObserver,
      SessionObserver
{
    public:
        GameServer();
        virtual ~GameServer();

        int start(char const* address, short port);
        int stop();
            
    protected:

        // SessionObserver interfaces.
        virtual void onCommand(Session* session, Command* command);
        virtual void onSessionClosed(Session* session);

        // ServerObserver interfaces.
        virtual void onNewConnection(Server* server, Connection* connection);
        virtual void onConnectionClosed(Server* server, Connection* connection);

    private:
        Server mServer;
        ServerMapIndex mMapIndex;

        // Called when LoginCommand is received.
        void onLogin(ServerSession* session, LoginCommand* cmd);
        
        typedef std::map< Connection*, ServerSession* > ConnectionSessionMap;
        ConnectionSessionMap mSessions;
};
