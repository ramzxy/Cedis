#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <vector>
#include <string>

class parser
{
    public:
    // adds received to internal buffer
    void addToBuffer(const std::vector<uint8_t> &data);

    // checks if the full command is sent
    bool isCommandValid();

    // parse the command and return
    std::vector<std::string> parse();

    private:
    std::vector<uint8_t> buffer_;
};

#endif PARSER_H
