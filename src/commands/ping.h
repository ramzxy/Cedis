#include <vector>

#include "command.h"

class ping : public command
{
public:
    ping() = default;
    std::string execute(const std::vector<std::string> &args) override
    {
        return "+PONG\r\n";
    }
};
