#ifndef GETCOMMAND_H
#define GETCOMMAND_H
#include "command.h"

class getCommand : public command {
public:
    std::string execute(const std::vector<std::string> &args,
                        std::shared_ptr<database> db_) override {
        if (args.size() != 2) return "Wrong number of arguments\r\n";

        return db_->get(args[1]) + "\r\n";
    }
};
#endif //GETCOMMAND_H
