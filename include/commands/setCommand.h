#ifndef SET_H
#define SET_H
#include "command.h"

class setCommand : public command
{
    using command::command;
    public:
        std::string execute(const std::vector<std::string>& args) override
        {
            if (args.size() != 3) return "Wrong number of arguments\r\n";

            db_.set(args[1], args[2]);
            return "+OK\r\n";
        }
};
#endif //SET_H
