#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
#include <string>
#include "database.h"

class command
{
public:
    command(database& db) : db_(db) {}
    virtual ~command() = default;

    virtual std::string execute(const std::vector<std::string>& args) = 0;

protected:
    database db_;
};

#endif COMMAND_H
