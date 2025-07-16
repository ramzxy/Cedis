
#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>

#include "connection.h"
#include "handleCMD.h"

using namespace boost::asio::ip;

class server {
public:
    server(boost::asio::io_context& io_context,
        int port,
        std::shared_ptr<handleCMD> handler)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      handler_(handler) {
        start_accept();
    }
private:

    void start_accept() {
        auto socket = std::make_shared<tcp::socket>(io_context_);

        acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                std::make_shared<Connection>(socket, handler_) -> handle_client();
            }
            start_accept();
        });
    }

    tcp::acceptor acceptor_;
    std::shared_ptr<handleCMD> handler_;
    boost::asio::io_context& io_context_;
};
#endif //SERVER_H
