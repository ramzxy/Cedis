
#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
#include <string>

class command
{
public:
    virtual ~command() = default;
    virtual std::string execute(const std::vector<std::string> &args) = 0;
};

#endif COMMAND_H
