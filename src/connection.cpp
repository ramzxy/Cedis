#include "connection.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "parser.h"
using namespace boost::asio::ip;


Connection::Connection(boost::asio::io_context &io_context,
                       const std::string &server_ip,
                       int server_port)
    : io_context_(io_context),
      socket_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), server_port)),
      server_port_(server_port),
      server_ip_(server_ip),
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

bool Connection::start()
{
    try
    {
        acceptor_.accept(socket_);

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

void Connection::handle_client()
{
    if (!connected_)
    {
        std::cerr << "Cannot handle client: not connected" << std::endl;
    }
    //TODO: add async and make sure these can run multi command.
    try
    {
        while (parser_.isCommandValid())
            parser_.addToBuffer(read());
        auto command = parser_.parse();
        //TODO: implement a command class
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error handling client: " << e.what() << std::endl;
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

int Connection::send_response(const std::string *response)
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
            boost::asio::buffer(*response));
        if (bytes_sent != 0)
        {
            std::cout << "Sent: " << *response << std::endl;
        }
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

std::vector<uint8_t> Connection::read()
{
    if (!connected_)
    {
        std::cerr << "Cannot receive data: not connected" << std::endl;
    }

    try
    {
        std::vector<uint8_t> buffer(1024);
        size_t bytes_read = 0;
        boost::system::error_code error;

        bytes_read = socket_.read_some(boost::asio::buffer(buffer), error);

        if (bytes_read != 0 && buffer[0] != 'X')
        {
            for (uint8_t byte : buffer) {
                if (byte == 0) break;
                std::cout << static_cast<char>(byte);
            }
            return buffer;
        }

        if (error)
        {
            if (error == boost::asio::error::eof)
            {
                std::cout << "Server closed the connection" << std::endl;
                connected_ = false;
            }
            else
            {
                throw boost::system::system_error(error);
            }
        }

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
    }
}

bool Connection::is_connected() const
{
    return connected_ && socket_.is_open();
}


