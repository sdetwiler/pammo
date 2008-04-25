#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

class Command;

class CommandObserver
{
    public:
        CommandObserver()
        {}

        virtual ~CommandObserver()
        {}

        virtual void onExecuted(Command* command) = 0;
};


class Command
{
    public:
        
        Command();
        virtual ~Command();

        void setObserver(CommandObserver* o);
        CommandObserver* getObserver();

        virtual uint32_t getId() = 0;
        virtual int execute() = 0;
        virtual int serialize(uint8_t** data, uint32_t* dataLen)=0;
        virtual int deserialize(uint8_t* data, uint32_t dataLen)=0;
        
    protected:
    private:
        CommandObserver* mObserver;
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
