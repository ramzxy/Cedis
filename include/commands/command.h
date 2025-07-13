#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
#include <string>
#include "../database.h"

class command {
public:
    virtual ~command() = default;

    virtual std::string execute(const std::vector<std::string> &args,
                                std::shared_ptr<database> db_) = 0;

protected:
    database db_;
};

#endif COMMAND_H
