//
// Created by Ilia on 30/06/2025.
//

#ifndef COMMAND_H
#define COMMAND_H
class command
{
    virtual ~command() = default;
public:
    virtual std::string execute(const std::vector<std::string> &args) = 0;
};

#endif //COMMAND_H
