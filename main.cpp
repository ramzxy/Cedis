#include <iostream>
#include "connection.h"
#include <boost/asio.hpp>

using namespace boost::asio;

int main() {
	std::string server_ip = "127.0.0.1";
	int server_port = 8090;

	std::cout << "Hello, Boost.Asio!" << std::endl;
	io_context io_context; 


	auto connection = std::make_shared<Connection>(io_context, server_ip, server_port);
}	