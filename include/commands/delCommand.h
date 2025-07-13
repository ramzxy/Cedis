#ifndef DELCOMMAND_H
#define DELCOMMAND_H
#include "command.h"

class delCommand : public command {
public:
    std::string execute(const std::vector<std::string> &args,
                        std::shared_ptr<database> db_) override {
        if (args.size() != 2) return "Wrong number of arguments\r\n";

        return db_->del(args[1]) ? "+OK\r\n" : "-ERR\r\n";
    }
};
#endif //DELCOMMAND_H
