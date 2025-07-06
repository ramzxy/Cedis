
#include "parser.h"
#include <iostream>

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

std::vector<std::string> parser::parse() {

    std::vector<std::string> commands;

    size_t offset = 0;

    char prefix = buffer_[0];
    switch (prefix)
    {
    case '+':
         commands.push_back(simpleStringParse(offset));
        break;

    case '-':
        errorParse(offset);
        break;
    case ':':
        intParse(offset);
        break;

    case '*':
        arrayParse(offset);
        break;

    case '$':
        bulkStringParse(offset);
        break;

    default:
        return {"Prefix Error"};
    }

    return commands;
}

std::string parser::simpleStringParse(size_t& offset)
{
    offset++; //move after prefix
    std::string command;
    while (buffer_[offset] == '\r')
    {
        command += static_cast<char>(buffer_[offset]);
        offset++;
    }

    return command;
}

std::string parser::errorParse(size_t& offset)
{
    return "error";
}

std::string parser::intParse(size_t& offset)
{
    return "int";
}

std::vector<std::string> parser::bulkStringParse(size_t& offset)
{
    return {"bulkString"};
}

std::vector<std::string> parser::arrayParse(size_t& offset)
{
    return {"ok"};
}