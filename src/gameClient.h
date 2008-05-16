#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "types.h"

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

        // Login to a specific map instance.
        void login(MapInstanceId const& id);

        bool isConnected();
            
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

        // Called when server sends an error command to the client.
        void onError(ErrorCommand* cmd);
        
        Client mClient;
        Session* mSession;
        bool mConnected;
};


#endif // GAMECLIENT_H
