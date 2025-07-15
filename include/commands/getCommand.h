#ifndef GETCOMMAND_H
#define GETCOMMAND_H
#include "command.h"

class getCommand : public command {
public:
    std::string execute(const std::vector<std::string> &args,
                        std::shared_ptr<database> db_) override {
        if (args.size() != 2) {
            return "-ERR wrong number of arguments for 'get' command\r\n";
        }

        try {
            std::string value = db_->get(args[1]);
            // Return proper Redis bulk string format
            return "$" + std::to_string(value.length()) + "\r\n" + value + "\r\n";
        } catch (const std::runtime_error&) {
            // Return Redis null bulk string for missing keys
            return "$-1\r\n";
        }
    }
};
#endif //GETCOMMAND_H
