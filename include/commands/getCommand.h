//
// Created by Ilia on 11/07/2025.
//

#ifndef GETCOMMAND_H
#define GETCOMMAND_H
#include "command.h"

class getCommand : public command
{
    using command::command;
    public:
        std::string execute(const std::vector<std::string>& args) override
        {
            if (args.size() != 2) return "Wrong number of arguments\r\n";

            return db_.get(args[1]) + "\r\n";
        }
};
#endif //GETCOMMAND_H
