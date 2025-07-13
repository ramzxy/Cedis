#ifndef HANDLECMD_H
#define HANDLECMD_H
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "commands/command.h"
#include "commands/pingCommand.h"
#include "commands/echoCommand.h"
#include "commands/setCommand.h"


class handleCMD
{
public:
    handleCMD(std::shared_ptr<database> db);

    std::string handle(const std::vector<std::string>& input);

    std::shared_ptr<database> db_;

private:
    std::unordered_map<std::string, std::unique_ptr<command>> command_;
};
#endif HANDLECMD_H
