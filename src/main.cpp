#include <iostream>
#include "connection.h"
#include "server.h"
#include <boost/asio.hpp>
#include "database.h"
#include "handleCMD.h"

using namespace boost::asio;

int main()
{
    std::string server_ip = "127.0.0.1";
    int server_port = 6969;

    std::cout << "Cedis, Onboard!" << std::endl;

    // Create a shared databasae instance
    auto db = std::make_shared<database>();

    // Create a shared command handler that uses the same database
    auto handle = std::make_shared<handleCMD>(db);

    io_context io_context;

    server server_(io_context, server_port, handle);

    io_context.run();

    db->save_local_store();
}
