﻿#include <iostream>
#include "connection.h"
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio;

int main() {
	std::string server_ip = "127.0.0.1";
	int server_port = 6969;

	std::cout << "Hello, Boost.Asio!" << std::endl;
	io_context io_context; 

	auto connection = std::make_shared<Connection>(io_context, server_ip, server_port);

	if (!connection->start()) {
		std::cerr << "Connection failed.";
	}
	std::string data = "Hello\n";
	std::vector<uint8_t> packet(data.begin(), data.end());

	if (!connection->send_response(&data)) {
		std::cerr << "packet send failed.";
	}

	connection->handle_client();
}	