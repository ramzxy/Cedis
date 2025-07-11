//
// Created by Ilia on 11/07/2025.
//

#ifndef EXISTSCOMMAND_H
#define EXISTSCOMMAND_H
#include "command.h"

class existsCommand : public command
{
    using command::command;
    public:
        std::string execute(const std::vector<std::string>& args) override
        {
            if (args.size() != 2) return "Wrong number of arguments\r\n";

            return db_.exists ? "+OK\r\n" : "-ERR\r\n";
        }
};
#endif //EXISTSCOMMAND_H
