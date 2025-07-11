#ifndef HANDLECMD_H
#define HANDLECMD_H
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "command.h"
#include "commands/pingCommand.h"
#include "commands/echoCommand.h"
#include "commands/setCommand.h"


class handleCMD
{
public:
    handleCMD(database &db)
    {
        command_["PING"] = std::make_unique<pingCommand>(db);
        command_["ECHO"] = std::make_unique<echoCommand>(db);
        command_["SET"] = std::make_unique<setCommand>(db);
    }

    std::string handle(const std::vector<std::string>& input);

private:
    std::unordered_map<std::string, std::unique_ptr<command>> command_;
};
#endif HANDLECMD_H
