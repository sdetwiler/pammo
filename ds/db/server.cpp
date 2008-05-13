#include "server.h"

///////////////////////////////////////////////////////////////
namespace ps
{
    namespace ds
    {
        namespace db
        {
///////////////////////////////////////////////////////////////

Server::Server()
{

}


Server::~Server()
{

}


void Server::setHost(std::string const& host)
{
    mHost = host;
}

void Server::setHost(char const* host)
{
    mHost = host;
}


std::string const* Server::getHost() const
{
    return &mHost;
}
 

void Server::setUsername(std::string const& un)
{
    mUsername = un; 
}

void Server::setUsername(char const* un)
{
    mUsername = un; 
}


std::string const* Server::getUsername() const
{
    return &mUsername;
}


void Server::setPassword(std::string const& pw)
{
    mPassword = pw;
}
void Server::setPassword(char const* pw)
{
    mPassword = pw;
}


std::string const* Server::getPassword() const
{
    return &mPassword;
}
///////////////////////////////////////////////////////////////
        }
    }
}
///////////////////////////////////////////////////////////////
