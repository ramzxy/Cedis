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

    // Add basic validation
    if (buffer_.empty()) {
        return {"ERR empty buffer"};
    }

    try {
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
            commands =  arrayParse(offset);
            break;

        default:
            return {"ERR unknown prefix: " + std::string(1, prefix)};
        }
        buffer_.erase(buffer_.begin(), buffer_.begin() + offset);
        return commands;
    } catch (const std::runtime_error& e) {
        // Clear buffer on parsing error to prevent getting stuck
        buffer_.clear();
        return {"ERR " + std::string(e.what())};
    } catch (const std::exception& e) {
        buffer_.clear();
        return {"ERR parsing exception: " + std::string(e.what())};
    }
}

std::string parser::simpleStringParse(size_t& offset)
{
    offset++; //skip prefix
    
    // Add bounds checking
    if (offset >= buffer_.size()) {
        throw std::runtime_error("Unexpected end of buffer while parsing simple string");
    }
    
    std::string command;
    while (offset < buffer_.size() && buffer_[offset] != '\r')
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

    // Add bounds checking
    if (offset >= buffer_.size()) {
        throw std::runtime_error("Unexpected end of buffer while parsing bulk string length");
    }

    while (offset < buffer_.size() && buffer_[offset] != '\r')
    {
        numCharStr += static_cast<char>(buffer_[offset++]); //parse num of chars
    }

    // Validate we found \r
    if (offset >= buffer_.size()) {
        throw std::runtime_error("Missing \\r in bulk string length");
    }

    int numChar;
    try {
        numChar = std::stoi(numCharStr);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid bulk string length: " + numCharStr);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Bulk string length out of range: " + numCharStr);
    }


    offset += 2; //skip \r\n

    // Validate we have enough buffer for the string
    if (offset + numChar > buffer_.size()) {
        throw std::runtime_error("Not enough buffer data for bulk string of length " + std::to_string(numChar));
    }

    std::string command(reinterpret_cast<const char*>(&buffer_[offset]), numChar);

    offset += numChar; //skip string and \r\n
    return command;
}

std::vector<std::string> parser::arrayParse(size_t& offset)
{
    std::string numArrStr;
    std::vector<std::string> commands_;

    offset++; //skip prefix

    // Add bounds checking
    if (offset >= buffer_.size()) {
        throw std::runtime_error("Unexpected end of buffer while parsing array length");
    }

    while (offset < buffer_.size() && buffer_[offset] != '\r')
    {
        numArrStr += static_cast<char>(buffer_[offset++]); //parse num of elements
    }

    // Validate we found \r
    if (offset >= buffer_.size()) {
        throw std::runtime_error("Missing \\r in array length");
    }

    int numArr;
    try {
        numArr = std::stoi(numArrStr);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid array length: " + numArrStr);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Array length out of range: " + numArrStr);
    }


    offset += 2; //skip \r\n

    for (int i = 0; i < numArr; i++)
    {
        // Bounds check before accessing buffer
        if (offset >= buffer_.size()) {
            throw std::runtime_error("Unexpected end of buffer while parsing array element " + std::to_string(i));
        }

        switch (buffer_[offset])
        {
        case '+':
            commands_.push_back(simpleStringParse(offset));
            break;

        case '$':
            commands_.push_back(bulkStringParse(offset));
            break;
            
        default:
            throw std::runtime_error("Unsupported array element type: " + std::string(1, buffer_[offset]));
        }
        offset += 2; //skip \r\n
    }

    return commands_;
}
