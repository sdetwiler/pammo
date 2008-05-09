#include "client.h"

#include "session.h"

class GameClient
    : public ClientObserver,
      public SessionObserver
{
    public:
        GameClient();
        virtual ~GameClient();

        int connect(char const* address, short port);
        int disconnect();

        void login();
        
            
    protected:

        // ClientObserver interfaces.
        virtual void onClientConnected(Client* client, Connection* connection);
        virtual void onClientDisconnected(Client* client, Connection* connection);

        // SessionObserver interfaces.
        virtual void onCommand(Session* session, Command* command);
        virtual void onSessionClosed(Session* session);
        
    private:

        // Called when server sends a status update command to the
        // client.
        void onStatusUpdate(StatusUpdateCommand* cmd);
        
        
        Client mClient;
        Session* mSession;
};
