#ifndef __TYPES_PLATFORM_H__
#define __TYPES_PLATFORM_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>

inline int getSocketError(){return errno;}

#endif // __TYPES_PLATFORM_H__