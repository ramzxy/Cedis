#ifndef HANDLECMD_H
#define HANDLECMD_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "command.h"
#include "commands/ping.h"
#include "commands/echo.h"


class handleCMD
{
    public:
    handleCMD()
    {
        command_["PING"] = std::make_unique<ping>();
        command_["ECHO"] = std::make_unique<echo>();
    }

    std::string execute(const std::vector<std::string> &input);

    private:
    std::unordered_map<std::string, std::unique_ptr<command>> command_;
};
#endif HANDLECMD_H
