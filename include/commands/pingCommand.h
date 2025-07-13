#ifndef PING_H
#define PING_H
#include "command.h"

class pingCommand : public command
{
    using command::command;
public:
    std::string execute(const std::vector<std::string>& args,
        std::shared_ptr<database> db_) override
    {
        return "+PONG\r\n";
    }
};

#endif PING_H
