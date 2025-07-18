#ifndef CONNECTION_H
#define CONNECTION_H
#pragma once

#include <string>
#include <boost/asio.hpp>

#include "database.h"
#include "handleCMD.h"
#include "parser.h"

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    Connection(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
               std::shared_ptr<handleCMD>);

    ~Connection();

    void handle_client();

    void send_response(const std::string& response);

    void read();

    bool is_connected() const;

    void disconnect();

private:
    // Boost ASIO components for networking
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;

    // Connection state
    bool connected_;

    // Parser object
    parser parser_;

    // command handler object
    std::shared_ptr<handleCMD> handler_;

    std::vector<uint8_t> buffer_;
};

#endif  CONNECTION_H
