#include "player.h"

Player::Player(ServerSession* session, uint32_t id)
{
    mSession = session;
    mId = id;
}


Player::~Player()
{}


void Player::send(Command* command)
{
    mSession->send(command);
}
