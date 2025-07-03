#ifndef PING_H
#define PING_H
#include "command.h"

class ping : public command
{
public:
    std::string execute(const std::vector<std::string> &args) override
    {
        return "+PONG\r\n";
    }
};

#endif PING_H