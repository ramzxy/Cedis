#ifndef ECHO_H
#define ECHO_H
#include "command.h"


class echo : public command
{
public:
    std::string execute(const std::vector<std::string>& args) override
    {
        return args[1];
    }
};



#endif ECHO_H
