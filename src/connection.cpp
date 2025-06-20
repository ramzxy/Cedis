#include "connection.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

Connection::Connection(boost::asio::io_context &io_context,
                       const std::string &server_ip,
                       int server_port)
    : io_context_(io_context),
      socket_(io_context),
      server_ip_(server_ip),
      server_port_(server_port),
      connected_(false)

{

    std::cout << "Connection object initialized with server: "
              << server_ip << ":" << server_port << std::endl;
}

Connection::~Connection()
{

    if (connected_)
    {
        disconnect();
    }

    std::cout << "Connection object destroyed" << std::endl;
}

bool Connection::connect()
{
    try
    {

        boost::asio::ip::tcp::resolver resolver(io_context_);

        boost::asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve(server_ip_, std::to_string(server_port_));

        std::cout << "Resolved server address, attempting connection..." << std::endl;

        boost::asio::connect(socket_, endpoints);

        connected_ = true;
        std::cout << "TCP connection established to "
                  << server_ip_ << ":" << server_port_ << std::endl;

        std::cout << "VPN connection established successfully" << std::endl;
        return true;    }
    catch (const boost::system::system_error &e)
    {

        std::cerr << "Connection error: " << e.what() << std::endl;
        connected_ = false;
        return false;
    }
}

void Connection::disconnect()
{
    if (!connected_)
    {
        return; // Already disconnected
    }

    try
    {

        // This is a courtesy to let the server know we're disconnecting
        std::string disconnect_msg = "DISCONNECT";
        boost::asio::write(socket_, boost::asio::buffer(disconnect_msg));

        socket_.close();

        connected_ = false;
        std::cout << "Disconnected from VPN server" << std::endl;
    }
    catch (const boost::system::system_error &e)
    {
        // If there's an error during disconnect, just log it
        std::cerr << "Error during disconnect: " << e.what() << std::endl;
        connected_ = false;
    }
}

int Connection::send_data(const uint8_t *data, size_t length)
{
    if (!connected_)
    {
        std::cerr << "Cannot send data: not connected" << std::endl;
        return -1;
    }

    try
    {
        // This will block until all data is sent
        size_t bytes_sent = boost::asio::write(
            socket_,
            boost::asio::buffer(data, length));

// For debugging in verbose mode
#ifdef DEBUG_MODE
        std::cout << "Sent " << bytes_sent << " bytes to server" << std::endl;
#endif

        return static_cast<int>(bytes_sent);
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error sending data: " << e.what() << std::endl;

        // If we get a connection error, mark as disconnected
        if (e.code() == boost::asio::error::connection_reset ||
            e.code() == boost::asio::error::broken_pipe)
        {
            connected_ = false;
        }

        return -1;
    }
}

int Connection::receive_data(uint8_t *data, size_t max_length)
{
    if (!connected_)
    {
        std::cerr << "Cannot receive data: not connected" << std::endl;
        return -1;
    }

    try
    {
        // This will block until at least some data is available
        boost::system::error_code error;
        size_t bytes_received = socket_.read_some(
            boost::asio::buffer(data, max_length),
            error);

        if (error)
        {
            if (error == boost::asio::error::eof)
            {
                std::cout << "Server closed the connection" << std::endl;
                connected_ = false;
                return 0;
            }
            else
            {
                throw boost::system::system_error(error);
            }
        }

// For debugging in verbose mode
#ifdef DEBUG_MODE
        std::cout << "Received " << bytes_received << " bytes from server" << std::endl;
#endif

        return static_cast<int>(bytes_received);
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error receiving data: " << e.what() << std::endl;

        // If we get a connection error, mark as disconnected
        if (e.code() == boost::asio::error::connection_reset ||
            e.code() == boost::asio::error::broken_pipe)
        {
            connected_ = false;
        }

        return -1;
    }
}

bool Connection::is_connected() const
{
    return connected_ && socket_.is_open();
}


