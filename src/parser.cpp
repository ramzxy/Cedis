
#include "parser.h"

void parser::addToBuffer(const std::vector<uint8_t> &data) {
    buffer_.insert(buffer_.end(),
        data.begin(), data.end());
}

bool parser::isCommandValid() {
    if ( buffer_[buffer_.size() - 1] == '\n'
    && buffer_[buffer_.size() - 2] == '\r')
        return true;
    return false;
}

std::vector<std::string> parser::parse() {
    int hi = 0;
}