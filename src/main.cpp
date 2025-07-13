#include <iostream>
#include "connection.h"
#include <boost/asio.hpp>
#include "database.h"
#include "handleCMD.h"

using namespace boost::asio;

int main()
{
    std::string server_ip = "127.0.0.1";
    int server_port = 6969;

    std::cout << "Hello, Boost.Asio!" << std::endl;
    io_context io_context;

    // Create a shared databasae instance
    auto db = std::make_shared<database>();

    // Create a shared command handler that uses the same database
    auto handle = std::make_shared<handleCMD>(db);

    auto connection = std::make_shared<Connection>(io_context,
        server_ip,
        server_port,
        handle);

    if (!connection->start())
    {
        std::cerr << "Connection failed.";
    }

    connection->handle_client();

    db->save_local_store();
}
