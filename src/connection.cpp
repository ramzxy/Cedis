#include "connection.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "handleCMD.h"
#include "parser.h"

using namespace boost::asio::ip;

Connection::Connection(std::shared_ptr<tcp::socket> socket,
                       std::shared_ptr<handleCMD> handler)
    : socket_(std::move(socket)),
      connected_(false),
      handler_(std::move(handler)){}

Connection::~Connection() {
    if (connected_) {
        disconnect();
    }

    std::cout << "Connection object destroyed" << std::endl;
}

void Connection::handle_client() {
    std::string client_ip = socket_->remote_endpoint().address().to_string();
    std::string client_port = std::to_string(socket_->remote_endpoint().port());

    std::cout << "Client connected from " <<
        client_ip << ":" << client_port << std::endl;
    read();
}

void Connection::read() {
    auto self = shared_from_this();
    buffer_.resize(1024);

    socket_->async_read_some(boost::asio::buffer(buffer_),
    [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            buffer_.resize(length);
            parser_.addToBuffer(buffer_);

            while (parser_.isCommandValid()) {
                auto command = parser_.parse();

                if (!command.empty()) {
                    if (command[0] == "QUIT") {
                        disconnect();
                        return;
                    }

                    if (command[0].substr(0, 3) == "ERR") {
                        std::string error_response = "-" + command[0] + "\r\n";
                        send_response(error_response);
                        continue;
                    }

                    std::cout << "Received command: ";
                    for (const auto& part : command)
                        std::cout << part << " ";
                    std::cout << std::endl;

                    std::string response = handler_->handle(command);
                    send_response(response);
                }
            }
            // Continue reading
            read();
        } else {
            std::cerr << "Read error: " << ec.message() << std::endl;
            disconnect();
        }
    });
}

void Connection::send_response(const std::string &response) {
    auto self = shared_from_this();

    boost::asio::async_write(*socket_, boost::asio::buffer(response),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (ec) {
                disconnect();
            }
        });
}


void Connection::disconnect() {
    if (!connected_) {
        return; // Already disconnected
    }

    try {
        // This is a courtesy to let the server know we're disconnecting

        connected_ = false;
        boost::system::error_code ignored;

        socket_ -> shutdown(tcp::socket::shutdown_both, ignored);
        socket_ -> close(ignored);

        std::cout << "Disconnected from Redis server" << std::endl;

    } catch (const boost::system::system_error &e) {
        // If there's an error during disconnect, just log it
        std::cerr << "Error during disconnect: " << e.what() << std::endl;
        connected_ = false;
    }
}

bool Connection::is_connected() const {
    return connected_ && socket_->is_open();
}
