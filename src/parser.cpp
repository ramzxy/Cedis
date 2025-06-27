
#include "parser.h"

void parser::addToBuffer(const std::vector<uint8_t> &data) {
    buffer_.insert(buffer_.end(), data.begin(), data.end());
}

bool parser::isCommandValid() {
    if (buffer_.size() < 2) return false;

    if ( buffer_[buffer_.size() - 1] == '\n'
    && buffer_[buffer_.size() - 2] == '\r')
        return true;
    return false;
}
//TODO: Fix parser
std::vector<std::string> parser::parse() {
    std::vector<std::string> commands;
    size_t pindex = 0;

    std::string sign(buffer_[pindex], 1);
    commands.push_back(sign);

    while (buffer_[pindex] != '\r') pindex++;
    std::string command(buffer_[1], buffer_[pindex+1]);
    commands.push_back(command);
    return commands;
}