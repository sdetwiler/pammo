#ifndef SESSION_H
#define SESSION_H

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


        State    mInState;
        uint8_t  mInHeader[SIZEOF_PROTOCOL_HEADER];
        uint8_t  mInOffset;

        Command* mInCommand;
        
        uint16_t mInProtocolVersion; // 1.
        uint32_t mInCommandId;  // Identifies command.
        uint32_t mInPayloadLen; // Length of payload in bytes.
        uint8_t* mInPayload;


        SessionObserver* mObserver;
        
        Connection* mConnection;
};

#endif // SESSION_H
