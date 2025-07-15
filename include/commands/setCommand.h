#ifndef SET_H
#define SET_H
#include "command.h"

class setCommand : public command {
public:
    std::string execute(const std::vector<std::string> &args,
                        std::shared_ptr<database> db_) override {
        if (args.size() != 3) {
            return "-ERR wrong number of arguments for 'set' command\r\n";
        }

        db_->set(args[1], args[2]);
        return "+OK\r\n";
    }
};
#endif //SET_H
