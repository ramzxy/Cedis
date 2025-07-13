#include "handleCMD.h"

#include <algorithm>

handleCMD::handleCMD(std::shared_ptr<database> db) : db_(db) {
    command_["PING"] = std::make_unique<pingCommand>();
    command_["ECHO"] = std::make_unique<echoCommand>();
    command_["SET"] = std::make_unique<setCommand>();
    command_["GET"] = std::make_unique<getCommand>();
    command_["EXISTS"] = std::make_unique<existsCommand>();
    command_["DEL"] = std::make_unique<delCommand>();
}

std::string handleCMD::handle(const std::vector<std::string> &input) {
    if (input.empty()) return "Command is empty\r\n";

    std::string cmd = input[0];

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    auto it = command_.find(cmd);
    if (it == command_.end()) return "Command not found\r\n";

    return it->second->execute(input, db_);
}
