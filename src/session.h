#ifndef SESSION_H
#define SESSION_H

#include <queue>

#include "connection.h"
#include "command.h"


/*********************

version  | command  | payloadlen | payload
uint16_t | uint32_t | uint32_t   | ...

********************/

#define PROTOCOL_VERSION 0
#define SIZEOF_PROTOCOL_VERSION 2

#define PROTOCOL_COMMANDID (PROTOCOL_VERSION + SIZEOF_PROTOCOL_VERSION)
#define SIZEOF_PROTOCOL_COMMANDID 4

#define PROTOCOL_PAYLOADLEN ( PROTOCOL_COMMANDID + SIZEOF_PROTOCOL_COMMANDID )
#define SIZEOF_PROTOCOL_PAYLOADLEN 4


#define PROTOCOL_PAYLOAD ( PROTOCOL_PAYLOADLEN + SIZEOF_PROTOCOL_PAYLOADLEN )

#define SIZEOF_PROTOCOL_HEADER PROTOCOL_PAYLOAD



class ProtocolHeader
{
    public:
        ProtocolHeader()
        {}
        
        ~ProtocolHeader()
        {}

        static int serialize(uint8_t* data, uint32_t dataLen,
                             uint32_t commandId, uint32_t payloadLen)
        {
            if(dataLen < SIZEOF_PROTOCOL_HEADER)
            {
                return -1;
            }

            *((uint16_t*)(data+PROTOCOL_VERSION)) = htons(1);
            *((uint32_t*)(data+PROTOCOL_COMMANDID)) = htonl(commandId);
            *((uint32_t*)(data+PROTOCOL_PAYLOADLEN)) = htonl(payloadLen);

            return 0;
        }
};


class Session;

class SessionObserver
{
    public:
        SessionObserver()
        {}
        
        virtual ~SessionObserver()
        {}

        virtual void onCommand(Session* session, Command* command) = 0;
        virtual void onSessionClosed(Session* session) = 0;
};


class Session
    : public ConnectionObserver
{
    public:
        Session(Connection* connection);
        virtual ~Session();

        void setObserver(SessionObserver* observer);
        SessionObserver* getObserver();

        void send(Command* command);
        
    protected:
        virtual void onReadable(Connection* connection);
        virtual void onWritable(Connection* connection);
        virtual void onError(Connection* connection);
        virtual void onClosed(Connection* connection);
        
    private:

        void read();
        void write();
        
        enum State
        {
            Header = 0,
            Payload
        };


        State     mInState;
        uint8_t   mInHeader[SIZEOF_PROTOCOL_HEADER];
        uint32_t  mInOffset;

        Command*  mInCommand;
        
        uint16_t  mInProtocolVersion; // 1.
        uint32_t  mInCommandId;  // Identifies command.
        uint32_t  mInPayloadLen; // Length of payload in bytes.
        uint8_t*  mInPayload;

        typedef std::queue< Command* > CommandQueue;
        CommandQueue mCommands;

        State    mOutState;
        uint8_t  mOutHeader[SIZEOF_PROTOCOL_HEADER];
        uint32_t mOutOffset;
        uint8_t* mOutPayload;
        uint32_t mOutPayloadLen;
        Command* mOutCommand;
        

        SessionObserver* mObserver;
        
        Connection* mConnection;
};

#endif // SESSION_H
