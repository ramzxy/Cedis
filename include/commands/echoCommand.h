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
        return args[1] + "\r\n";
    }
};


#endif ECHO_H
