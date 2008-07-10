#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"

class Command;

class CommandObserver
{
    public:
        CommandObserver()
        {}

        virtual ~CommandObserver()
        {}

        virtual void onSent(Command* command) = 0;
};


class Command
{
    public:
        
        Command();
        virtual ~Command();

        void setObserver(CommandObserver* o);
        CommandObserver* getObserver();

        virtual uint32_t getId() = 0;
        virtual uint32_t getPayloadLength() = 0;
        
        virtual int serialize(uint8_t* data, uint32_t dataLen)=0;
        virtual int deserialize(uint8_t* data, uint32_t dataLen)=0;
        
    protected:
    private:
        CommandObserver* mObserver;
};

// Sent from client to server to make a login attempt.
#define PAMMO_COMMAND_LOGIN     1

class LoginCommand :
    public Command
{
    public:
        LoginCommand();
        virtual ~LoginCommand();

        virtual uint32_t getId()
        {
            return PAMMO_COMMAND_LOGIN;
        }

        virtual uint32_t getPayloadLength()
        {
            return sizeof(mMapInstanceId);
        }
        
        virtual int serialize(uint8_t* data, uint32_t dataLen);
        virtual int deserialize(uint8_t* data, uint32_t dataLen);

        MapInstanceId const& getMapInstanceId() const;
        void setMapInstanceId(MapInstanceId const& id);
        
    private:
        MapInstanceId mMapInstanceId;
};

// Sent from server to client to update the client's status.
// Status can be:
//   Logged in
//   Logged out
//
#define PAMMO_COMMAND_STATUSUPDATE  2
class StatusUpdateCommand :
    public Command
{
    public:
        StatusUpdateCommand();
        virtual ~StatusUpdateCommand();

        virtual uint32_t getId()
        {
            return PAMMO_COMMAND_STATUSUPDATE;
        }

        virtual uint32_t getPayloadLength()
        {
            return 0;
        }
        
        virtual int serialize(uint8_t* data, uint32_t dataLen);
        virtual int deserialize(uint8_t* data, uint32_t dataLen);
};

#define PAMMO_COMMAND_ERROR  3
class ErrorCommand :
    public Command
{
    public:
        ErrorCommand();
        virtual ~ErrorCommand();

        virtual uint32_t getId()
        {
            return PAMMO_COMMAND_ERROR;
        }

        virtual uint32_t getPayloadLength()
        {
            return sizeof(int32_t);
        }

        virtual int serialize(uint8_t* data, uint32_t dataLen);
        virtual int deserialize(uint8_t* data, uint32_t dataLen);

        void setError(int32_t error);
        int32_t getError();

    private:
        int32_t mError;
};



class CommandFactory
{
    public:
        CommandFactory();
        virtual ~CommandFactory();

        static Command* newCommand(uint32_t id);
        static void deleteCommand(Command* command);
};

    


#endif // COMMAND_H