#ifndef DELCOMMAND_H
#define DELCOMMAND_H
#include "command.h"

class delCommand : public command {
public:
    std::string execute(const std::vector<std::string> &args,
                        std::shared_ptr<database> db_) override {
        if (args.size() != 2) {
            return "-ERR wrong number of arguments for 'del' command\r\n";
        }

        // Return number of keys deleted (0 or 1)
        bool deleted = db_->del(args[1]);
        return ":" + std::to_string(deleted ? 1 : 0) + "\r\n";
    }
};
#endif //DELCOMMAND_H
