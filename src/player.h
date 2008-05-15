#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#include "serverSession.h"

class Player
{
    public:
        Player(ServerSession* session, uint32_t id);
        virtual ~Player();

        void send(Command* command);

    private:
        ServerSession* mSession;
        uint32_t mId;
        // Vehicle.
        // Score.
        // Health.
        // Position.
        // Path.
};

#endif // PLAYER_H
