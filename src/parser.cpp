#include "parser.h"
#include <iostream>
#include <boost/asio/buffer.hpp>


void parser::addToBuffer(const std::vector<uint8_t>& data)
{
    buffer_.insert(buffer_.end(), data.begin(), data.end());
}

bool parser::isCommandValid()
{
    if (buffer_.size() < 2) return false;

    if (buffer_[buffer_.size() - 1] == '\n'
        && buffer_[buffer_.size() - 2] == '\r')
        return true;
    return false;
}

std::vector<std::string> parser::parse()
{
    std::vector<std::string> commands;
    size_t offset = 0;

    char prefix = buffer_[0];
    switch (prefix)
    {
    case '+':
        commands.push_back(simpleStringParse(offset));
        break;

    case '$':
        commands.push_back(bulkStringParse(offset));
        break;

    case '*':
        return arrayParse(offset);
        break;

    default:
        return {"Prefix Error"};
    }

    return commands;
}

std::string parser::simpleStringParse(size_t& offset)
{
    offset++; //skip prefix
    std::string command;
    while (buffer_[offset] != '\r')
    {
        command += static_cast<char>(buffer_[offset]);
        offset++;
    }

    return command;
}

std::string parser::bulkStringParse(size_t& offset)
{
    std::string numCharStr;

    offset++; //skip prefix

    while (buffer_[offset] != '\r')
    {
        numCharStr += static_cast<char>(buffer_[offset++]); //parse num of chars
    }
    int numChar = std::stoi(numCharStr);

    offset += 2; //skip \r\n

    std::string command(reinterpret_cast<const char*>(&buffer_[offset]), numChar);

    offset += numChar; //skip string and \r\n
    return command;
}

std::vector<std::string> parser::arrayParse(size_t& offset)
{
    std::string numArrStr;
    std::vector<std::string> commands_;

    offset++; //skip prefix

    while (buffer_[offset] != '\r')
    {
        numArrStr += static_cast<char>(buffer_[offset++]); //parse num of elements
    }
    int numArr = std::stoi(numArrStr);

    offset += 2; //skip \r\n

    for (int i = 0; i < numArr; i++)
    {
        switch (buffer_[offset])
        {
        case '+':
            commands_.push_back(simpleStringParse(offset));
            break;

        case '$':
            commands_.push_back(bulkStringParse(offset));
            break;
        }
        offset += 2; //skip \r\n
    }

    return commands_;
}
