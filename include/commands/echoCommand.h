#ifndef ECHO_H
#define ECHO_H
#include "command.h"


class echoCommand : public command
{
    using command::command;
public:
    std::string execute(const std::vector<std::string>& args,
        std::shared_ptr<database> db_) override
    {
        if (args.size() < 2) {
            return "-ERR wrong number of arguments for 'echo' command\r\n";
        }
        return args[1] + "\r\n";
    }
};


#endif ECHO_H
