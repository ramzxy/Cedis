//
// Created by Ilia on 11/07/2025.
//

#ifndef EXISTSCOMMAND_H
#define EXISTSCOMMAND_H
#include "command.h"

class existsCommand : public command
{

    public:
        std::string execute(const std::vector<std::string>& args,
            std::shared_ptr<database> db_) override
        {
            if (args.size() != 2) {
                return "-ERR wrong number of arguments for 'exists' command\r\n";
            }

            // Return integer 1 if exists, 0 if not (Redis format)
            bool exists = db_->exists(args[1]);
            return ":" + std::to_string(exists ? 1 : 0) + "\r\n";
        }
};
#endif //EXISTSCOMMAND_H
