# Cedis Integration Examples

## Table of Contents

- [Client Integration Examples](#client-integration-examples)
- [Custom Command Development](#custom-command-development)
- [Multi-Client Architecture](#multi-client-architecture)
- [Error Handling Patterns](#error-handling-patterns)
- [Performance Optimization](#performance-optimization)
- [Testing Strategies](#testing-strategies)

---

## Client Integration Examples

### C++ Client Integration

```cpp
// cedis_client.h
#ifndef CEDIS_CLIENT_H
#define CEDIS_CLIENT_H

#include <boost/asio.hpp>
#include <string>
#include <vector>

class CedisClient {
public:
    CedisClient(const std::string& host, int port);
    ~CedisClient();

    bool connect();
    void disconnect();

    // Basic operations
    bool ping();
    std::string echo(const std::string& message);
    bool set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);

private:
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket socket_;
    std::string host_;
    int port_;
    bool connected_;

    std::string send_command(const std::vector<std::string>& args);
    std::string read_response();
};

#endif
```

```cpp
// cedis_client.cpp
#include "cedis_client.h"
#include <iostream>
#include <sstream>

CedisClient::CedisClient(const std::string& host, int port)
    : socket_(io_context_), host_(host), port_(port), connected_(false) {}

CedisClient::~CedisClient() {
    if (connected_) disconnect();
}

bool CedisClient::connect() {
    try {
        boost::asio::ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host_, std::to_string(port_));
        boost::asio::connect(socket_, endpoints);
        connected_ = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
}

void CedisClient::disconnect() {
    if (connected_) {
        socket_.close();
        connected_ = false;
    }
}

std::string CedisClient::send_command(const std::vector<std::string>& args) {
    if (!connected_) return "";

    // Build RESP array command
    std::ostringstream cmd;
    cmd << "*" << args.size() << "\r\n";
    for (const auto& arg : args) {
        cmd << "$" << arg.length() << "\r\n" << arg << "\r\n";
    }

    // Send command
    boost::asio::write(socket_, boost::asio::buffer(cmd.str()));

    // Read response
    return read_response();
}

std::string CedisClient::read_response() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket_, buffer, "\r\n");

    std::istream response_stream(&buffer);
    std::string response;
    std::getline(response_stream, response);

    // Remove \r if present
    if (!response.empty() && response.back() == '\r') {
        response.pop_back();
    }

    return response;
}

bool CedisClient::ping() {
    std::string response = send_command({"PING"});
    return response == "+PONG";
}

std::string CedisClient::echo(const std::string& message) {
    std::string response = send_command({"ECHO", message});
    return response.substr(1); // Remove '+' prefix
}

bool CedisClient::set(const std::string& key, const std::string& value) {
    std::string response = send_command({"SET", key, value});
    return response == "+OK";
}

std::string CedisClient::get(const std::string& key) {
    std::string response = send_command({"GET", key});
    if (response.empty() || response[0] == '-') return ""; // Error case
    return response.substr(1); // Remove '+' prefix
}

bool CedisClient::del(const std::string& key) {
    std::string response = send_command({"DEL", key});
    return response == ":1";
}

bool CedisClient::exists(const std::string& key) {
    std::string response = send_command({"EXISTS", key});
    return response == ":1";
}

// Usage example
int main() {
    CedisClient client("127.0.0.1", 6969);

    if (!client.connect()) {
        std::cerr << "Failed to connect to Cedis server" << std::endl;
        return 1;
    }

    // Test operations
    if (client.ping()) {
        std::cout << "Server is alive!" << std::endl;
    }

    client.set("greeting", "Hello Cedis!");
    std::string value = client.get("greeting");
    std::cout << "Retrieved: " << value << std::endl;

    client.disconnect();
    return 0;
}
```

### Python Client Integration

```python
# cedis_client.py
import socket
import struct

class CedisClient:
    def __init__(self, host='127.0.0.1', port=6969):
        self.host = host
        self.port = port
        self.socket = None
        self.connected = False

    def connect(self):
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.connected = True
            return True
        except Exception as e:
            print(f"Connection failed: {e}")
            return False

    def disconnect(self):
        if self.connected and self.socket:
            self.socket.close()
            self.connected = False

    def _send_command(self, *args):
        if not self.connected:
            return None

        # Build RESP array
        cmd = f"*{len(args)}\r\n"
        for arg in args:
            cmd += f"${len(str(arg))}\r\n{arg}\r\n"

        # Send command
        self.socket.send(cmd.encode())

        # Read response
        response = self.socket.recv(1024).decode().strip()
        return response

    def ping(self):
        response = self._send_command("PING")
        return response == "+PONG"

    def echo(self, message):
        response = self._send_command("ECHO", message)
        return response[1:] if response and response[0] == '+' else ""

    def set(self, key, value):
        response = self._send_command("SET", key, value)
        return response == "+OK"

    def get(self, key):
        response = self._send_command("GET", key)
        return response[1:] if response and response[0] == '+' else None

    def delete(self, key):
        response = self._send_command("DEL", key)
        return response == ":1"

    def exists(self, key):
        response = self._send_command("EXISTS", key)
        return response == ":1"

# Usage example
if __name__ == "__main__":
    client = CedisClient()

    if client.connect():
        print("Connected to Cedis!")

        # Test operations
        print("PING:", client.ping())

        client.set("user:1", "john_doe")
        print("GET user:1:", client.get("user:1"))

        print("EXISTS user:1:", client.exists("user:1"))
        print("DEL user:1:", client.delete("user:1"))
        print("EXISTS user:1:", client.exists("user:1"))

        client.disconnect()
    else:
        print("Failed to connect")
```

---

## Custom Command Development

### Advanced Command: INCR (Increment)

```cpp
// include/commands/incrCommand.h
#ifndef INCR_COMMAND_H
#define INCR_COMMAND_H
#include "command.h"

class incrCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        if (args.size() != 2) {
            return "-ERR wrong number of arguments for 'INCR' command\r\n";
        }

        const std::string& key = args[1];

        try {
            // Try to get existing value
            std::string current_str = db_->get(key);
            int current_value = std::stoi(current_str);
            int new_value = current_value + 1;

            db_->set(key, std::to_string(new_value));
            return ":" + std::to_string(new_value) + "\r\n";

        } catch (const std::runtime_error&) {
            // Key doesn't exist, start from 0
            db_->set(key, "1");
            return ":1\r\n";

        } catch (const std::invalid_argument&) {
            return "-ERR value is not an integer or out of range\r\n";
        }
    }
};
#endif
```

### Command with Multiple Arguments: MSET

```cpp
// include/commands/msetCommand.h
#ifndef MSET_COMMAND_H
#define MSET_COMMAND_H
#include "command.h"

class msetCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        // MSET key1 value1 key2 value2 ...
        if (args.size() < 3 || args.size() % 2 == 0) {
            return "-ERR wrong number of arguments for 'MSET' command\r\n";
        }

        // Set all key-value pairs
        for (size_t i = 1; i < args.size(); i += 2) {
            const std::string& key = args[i];
            const std::string& value = args[i + 1];
            db_->set(key, value);
        }

        return "+OK\r\n";
    }
};
#endif
```

### Command with Conditional Logic: SETNX (Set if Not Exists)

```cpp
// include/commands/setnxCommand.h
#ifndef SETNX_COMMAND_H
#define SETNX_COMMAND_H
#include "command.h"

class setnxCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        if (args.size() != 3) {
            return "-ERR wrong number of arguments for 'SETNX' command\r\n";
        }

        const std::string& key = args[1];
        const std::string& value = args[2];

        if (db_->exists(key)) {
            return ":0\r\n"; // Key already exists, not set
        } else {
            db_->set(key, value);
            return ":1\r\n"; // Key was set
        }
    }
};
#endif
```

### Registering New Commands

```cpp
// Add to handleCMD constructor in src/handleCMD.cpp
handleCMD::handleCMD(std::shared_ptr<database> db) : db_(db) {
    // Existing commands
    command_["PING"] = std::make_unique<pingCommand>();
    command_["ECHO"] = std::make_unique<echoCommand>();
    command_["SET"] = std::make_unique<setCommand>();
    command_["GET"] = std::make_unique<getCommand>();
    command_["DEL"] = std::make_unique<delCommand>();
    command_["EXISTS"] = std::make_unique<existsCommand>();

    // New commands
    command_["INCR"] = std::make_unique<incrCommand>();
    command_["MSET"] = std::make_unique<msetCommand>();
    command_["SETNX"] = std::make_unique<setnxCommand>();
}
```

---

## Multi-Client Architecture

### Server with Connection Pool

```cpp
// include/connection_manager.h
#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include "connection.h"
#include "database.h"
#include "handleCMD.h"

class ConnectionManager {
public:
    ConnectionManager(const std::string& host, int port,
                     std::shared_ptr<database> db, int max_connections = 10);
    ~ConnectionManager();

    bool start();
    void stop();

private:
    void accept_connections();
    void handle_connection(std::shared_ptr<Connection> conn);

    std::string host_;
    int port_;
    int max_connections_;
    std::shared_ptr<database> db_;
    std::shared_ptr<handleCMD> handler_;

    std::atomic<bool> running_;
    std::vector<std::thread> worker_threads_;
    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
};

#endif
```

```cpp
// src/connection_manager.cpp
#include "connection_manager.h"
#include <iostream>

ConnectionManager::ConnectionManager(const std::string& host, int port,
                                   std::shared_ptr<database> db, int max_connections)
    : host_(host), port_(port), max_connections_(max_connections),
      db_(db), running_(false) {
    handler_ = std::make_shared<handleCMD>(db);
}

ConnectionManager::~ConnectionManager() {
    stop();
}

bool ConnectionManager::start() {
    try {
        acceptor_ = std::make_unique<boost::asio::ip::tcp::acceptor>(
            io_context_,
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_)
        );

        running_ = true;

        // Start worker threads
        for (int i = 0; i < max_connections_; ++i) {
            worker_threads_.emplace_back([this]() {
                io_context_.run();
            });
        }

        // Start accepting connections
        accept_connections();

        std::cout << "Server started on " << host_ << ":" << port_
                  << " with " << max_connections_ << " worker threads" << std::endl;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to start server: " << e.what() << std::endl;
        return false;
    }
}

void ConnectionManager::accept_connections() {
    auto new_connection = std::make_shared<Connection>(
        io_context_, host_, port_, handler_
    );

    acceptor_->async_accept(new_connection->get_socket(),
        [this, new_connection](boost::system::error_code ec) {
            if (!ec && running_) {
                std::cout << "New client connected" << std::endl;
                handle_connection(new_connection);
                accept_connections(); // Accept next connection
            }
        });
}

void ConnectionManager::handle_connection(std::shared_ptr<Connection> conn) {
    // Handle client in separate thread/coroutine
    std::thread([conn]() {
        conn->handle_client();
    }).detach();
}

void ConnectionManager::stop() {
    running_ = false;
    io_context_.stop();

    for (auto& thread : worker_threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    worker_threads_.clear();
}
```

---

## Error Handling Patterns

### Robust Command Implementation

```cpp
// include/commands/robustCommand.h
#ifndef ROBUST_COMMAND_H
#define ROBUST_COMMAND_H
#include "command.h"
#include <stdexcept>
#include <iostream>

class robustCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        try {
            return safe_execute(args, db_);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument error: " << e.what() << std::endl;
            return "-ERR invalid argument: " + std::string(e.what()) + "\r\n";
        } catch (const std::runtime_error& e) {
            std::cerr << "Runtime error: " << e.what() << std::endl;
            return "-ERR runtime error: " + std::string(e.what()) + "\r\n";
        } catch (const std::exception& e) {
            std::cerr << "Unexpected error: " << e.what() << std::endl;
            return "-ERR unexpected error\r\n";
        } catch (...) {
            std::cerr << "Unknown error occurred" << std::endl;
            return "-ERR unknown error\r\n";
        }
    }

private:
    std::string safe_execute(const std::vector<std::string>& args,
                           std::shared_ptr<database> db_) {
        // Validate input
        if (args.empty()) {
            throw std::invalid_argument("empty command");
        }

        if (args.size() < 2) {
            throw std::invalid_argument("insufficient arguments");
        }

        // Validate key
        const std::string& key = args[1];
        if (key.empty()) {
            throw std::invalid_argument("empty key not allowed");
        }

        if (key.length() > 256) {
            throw std::invalid_argument("key too long");
        }

        // Your command logic here...
        return "+OK\r\n";
    }
};
#endif
```

### Database Error Handling

```cpp
// Enhanced database operations with better error handling
class enhanced_database : public database {
public:
    enum class ErrorCode {
        SUCCESS,
        KEY_NOT_FOUND,
        INVALID_KEY,
        STORAGE_FULL,
        IO_ERROR
    };

    struct Result {
        ErrorCode code;
        std::string value;
        std::string error_message;

        bool success() const { return code == ErrorCode::SUCCESS; }
    };

    Result safe_get(const std::string& key) {
        if (key.empty()) {
            return {ErrorCode::INVALID_KEY, "", "Key cannot be empty"};
        }

        try {
            std::string value = get(key);
            return {ErrorCode::SUCCESS, value, ""};
        } catch (const std::runtime_error&) {
            return {ErrorCode::KEY_NOT_FOUND, "", "Key not found"};
        }
    }

    Result safe_set(const std::string& key, const std::string& value) {
        if (key.empty()) {
            return {ErrorCode::INVALID_KEY, "", "Key cannot be empty"};
        }

        if (store_.size() >= MAX_KEYS && !exists(key)) {
            return {ErrorCode::STORAGE_FULL, "", "Database is full"};
        }

        try {
            set(key, value);
            return {ErrorCode::SUCCESS, "", ""};
        } catch (const std::exception& e) {
            return {ErrorCode::IO_ERROR, "", "Failed to store key-value pair"};
        }
    }

private:
    static const size_t MAX_KEYS = 1000000; // 1M keys limit
};
```

---

## Performance Optimization

### Connection Pooling and Reuse

```cpp
// Optimized connection handling
class OptimizedConnection : public Connection {
public:
    OptimizedConnection(boost::asio::io_context& io_context,
                       const std::string& server_ip,
                       int server_port,
                       std::shared_ptr<handleCMD> handler)
        : Connection(io_context, server_ip, server_port, handler),
          buffer_size_(8192) {

        // Pre-allocate buffers
        read_buffer_.reserve(buffer_size_);
        write_buffer_.reserve(buffer_size_);
    }

    // Optimized read with larger buffer
    size_t read_optimized(std::vector<uint8_t>& data) {
        read_buffer_.resize(buffer_size_);

        boost::system::error_code ec;
        size_t bytes_read = socket_.read_some(
            boost::asio::buffer(read_buffer_), ec
        );

        if (!ec && bytes_read > 0) {
            data.assign(read_buffer_.begin(), read_buffer_.begin() + bytes_read);
        }

        return bytes_read;
    }

    // Batched response sending
    void send_batched_responses(const std::vector<std::string>& responses) {
        write_buffer_.clear();

        for (const auto& response : responses) {
            write_buffer_.insert(write_buffer_.end(),
                               response.begin(), response.end());
        }

        boost::asio::write(socket_, boost::asio::buffer(write_buffer_));
    }

private:
    size_t buffer_size_;
    std::vector<uint8_t> read_buffer_;
    std::vector<uint8_t> write_buffer_;
};
```

### Memory-Optimized Database

```cpp
// Memory pool for string allocations
class MemoryOptimizedDatabase : public database {
public:
    MemoryOptimizedDatabase() {
        // Reserve space to avoid frequent reallocations
        store_.reserve(10000);
    }

    // Use string_view for read-only operations
    bool exists(std::string_view key) const {
        return store_.find(std::string(key)) != store_.end();
    }

    // Bulk operations for better performance
    void mset(const std::vector<std::pair<std::string, std::string>>& pairs) {
        for (const auto& [key, value] : pairs) {
            store_[key] = value;
        }
    }

    std::vector<std::string> mget(const std::vector<std::string>& keys) {
        std::vector<std::string> results;
        results.reserve(keys.size());

        for (const auto& key : keys) {
            auto it = store_.find(key);
            if (it != store_.end()) {
                results.push_back(it->second);
            } else {
                results.emplace_back(); // Empty string for missing keys
            }
        }

        return results;
    }
};
```

---

## Testing Strategies

### Unit Tests for Commands

```cpp
// test/test_commands.cpp
#include <gtest/gtest.h>
#include "../include/database.h"
#include "../include/commands/setCommand.h"
#include "../include/commands/getCommand.h"

class CommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        db_ = std::make_shared<database>();
        set_cmd_ = std::make_unique<setCommand>();
        get_cmd_ = std::make_unique<getCommand>();
    }

    std::shared_ptr<database> db_;
    std::unique_ptr<setCommand> set_cmd_;
    std::unique_ptr<getCommand> get_cmd_;
};

TEST_F(CommandTest, SetAndGetBasic) {
    std::vector<std::string> set_args = {"SET", "testkey", "testvalue"};
    std::string set_result = set_cmd_->execute(set_args, db_);
    EXPECT_EQ(set_result, "+OK\r\n");

    std::vector<std::string> get_args = {"GET", "testkey"};
    std::string get_result = get_cmd_->execute(get_args, db_);
    EXPECT_EQ(get_result, "+testvalue\r\n");
}

TEST_F(CommandTest, SetWrongArguments) {
    std::vector<std::string> args = {"SET", "onlykey"};
    std::string result = set_cmd_->execute(args, db_);
    EXPECT_EQ(result, "Wrong number of arguments\r\n");
}

TEST_F(CommandTest, GetNonexistentKey) {
    std::vector<std::string> args = {"GET", "nonexistent"};
    std::string result = get_cmd_->execute(args, db_);
    EXPECT_TRUE(result.find("Key not found") != std::string::npos);
}
```

### Integration Tests

```cpp
// test/test_integration.cpp
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "../include/connection.h"
#include "../include/database.h"
#include "../include/handleCMD.h"

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        db_ = std::make_shared<database>();
        handler_ = std::make_shared<handleCMD>(db_);

        // Start server in separate thread
        server_thread_ = std::thread([this]() {
            boost::asio::io_context io_context;
            connection_ = std::make_shared<Connection>(
                io_context, "127.0.0.1", 6970, handler_
            );

            if (connection_->start()) {
                connection_->handle_client();
            }
        });

        // Give server time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void TearDown() override {
        if (connection_) {
            connection_->disconnect();
        }
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

    std::shared_ptr<database> db_;
    std::shared_ptr<handleCMD> handler_;
    std::shared_ptr<Connection> connection_;
    std::thread server_thread_;
};

TEST_F(IntegrationTest, ClientServerCommunication) {
    // Use a simple client to test communication
    CedisClient client("127.0.0.1", 6970);
    ASSERT_TRUE(client.connect());

    EXPECT_TRUE(client.ping());
    EXPECT_TRUE(client.set("integration_test", "success"));
    EXPECT_EQ(client.get("integration_test"), "success");

    client.disconnect();
}
```

### Benchmark Tests

```cpp
// test/benchmark.cpp
#include <chrono>
#include <iostream>
#include "../include/database.h"

void benchmark_database_operations() {
    auto db = std::make_shared<database>();
    const int num_operations = 100000;

    // Benchmark SET operations
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        db->set("key" + std::to_string(i), "value" + std::to_string(i));
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "SET: " << num_operations << " operations in "
              << duration.count() << " microseconds" << std::endl;
    std::cout << "SET rate: " << (num_operations * 1000000.0 / duration.count())
              << " ops/sec" << std::endl;

    // Benchmark GET operations
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        try {
            db->get("key" + std::to_string(i));
        } catch (...) {
            // Key not found
        }
    }
    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "GET: " << num_operations << " operations in "
              << duration.count() << " microseconds" << std::endl;
    std::cout << "GET rate: " << (num_operations * 1000000.0 / duration.count())
              << " ops/sec" << std::endl;
}

int main() {
    benchmark_database_operations();
    return 0;
}
```

---

This comprehensive documentation covers all the major aspects of integrating with and extending the Cedis API. The examples show real-world usage patterns and provide a solid foundation for building applications that use Cedis as their data store.
