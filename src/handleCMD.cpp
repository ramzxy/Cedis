#include "handleCMD.h"

#include <algorithm>

std::string handleCMD::handle(const std::vector<std::string>& input)
{
    if (input.empty()) return "Command is empty\r\n";

    std::string cmd = input[0];

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    auto it = command_.find(cmd);
    if (it == command_.end()) return "Command not found\r\n";

    return it->second->execute(input);
}
