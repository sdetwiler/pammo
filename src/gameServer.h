#include "server.h"

#include "session.h"

class GameServer
    : public ServerObserver,
      public SessionObserver
{
    public:
        GameServer();
        virtual ~GameServer();

        int start(char const* address, short port);
        int stop();
            
    protected:

        // ServerObserver interfaces.
        virtual void onNewConnection(Server* server, Connection* connection);
        virtual void onConnectionClosed(Server* server, Connection* connection);

        // SessionObserver interfaces.
        virtual void onCommand(Session* session, Command* command);
        virtual void onSessionClosed(Session* session);
        
    private:
        Server mServer;

        // Called when LoginCommand is received.
        void onLogin(Session* session, LoginCommand* cmd);
        

        
        typedef std::map< Connection*, Session* > ConnectionSessionMap;
        ConnectionSessionMap mSessions;
};
