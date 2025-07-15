# Cedis API Documentation

## Table of Contents

- [Overview](#overview)
- [Core Classes](#core-classes)
  - [Database](#database)
  - [Connection](#connection)
  - [Parser](#parser)
  - [Command Handler](#command-handler)
- [Command System](#command-system)
  - [Base Command Interface](#base-command-interface)
  - [Available Commands](#available-commands)
- [Usage Examples](#usage-examples)
- [Error Handling](#error-handling)
- [Configuration](#configuration)

---

## Overview

Cedis is a high-performance, lightweight in-memory key-value store inspired by Redis, built in modern C++17 using Boost.Asio for asynchronous networking. The architecture follows a modular design with clear separation of concerns.

### Key Features

- **Asynchronous TCP networking** using Boost.Asio
- **RESP (Redis Serialization Protocol)** compatibility
- **Modular command system** with extensible command pattern
- **In-memory storage** with optional persistence
- **Thread-safe operations**

---

## Core Classes

### Database

The `database` class provides the core in-memory key-value storage functionality with persistence support.

#### Header Location

```cpp
#include "database.h"
```

#### Class Definition

```cpp
class database {
public:
    database();
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key) const;
    void save_local_store() const;
    void read_local_store();

private:
    std::unordered_map<std::string, std::string> store_;
};
```

#### Methods

##### `database()`

**Description**: Constructor that initializes the database and loads data from persistent storage.

**Usage**:

```cpp
auto db = std::make_shared<database>();
```

##### `void set(const std::string& key, const std::string& value)`

**Description**: Stores a key-value pair in the database.

**Parameters**:

- `key`: The key to store (string)
- `value`: The value to associate with the key (string)

**Usage**:

```cpp
db->set("username", "john_doe");
db->set("session_id", "abc123");
```

##### `std::string get(const std::string& key)`

**Description**: Retrieves the value associated with a key.

**Parameters**:

- `key`: The key to look up (string)

**Returns**: The value as a string

**Throws**: `std::runtime_error` if key is not found

**Usage**:

```cpp
try {
    std::string username = db->get("username");
    std::cout << "Username: " << username << std::endl;
} catch (const std::runtime_error& e) {
    std::cout << "Key not found: " << e.what() << std::endl;
}
```

##### `bool del(const std::string& key)`

**Description**: Deletes a key-value pair from the database.

**Parameters**:

- `key`: The key to delete (string)

**Returns**: `true` if key was deleted, `false` if key didn't exist

**Usage**:

```cpp
bool deleted = db->del("session_id");
if (deleted) {
    std::cout << "Session ID deleted successfully" << std::endl;
}
```

##### `bool exists(const std::string& key) const`

**Description**: Checks if a key exists in the database.

**Parameters**:

- `key`: The key to check (string)

**Returns**: `true` if key exists, `false` otherwise

**Usage**:

```cpp
if (db->exists("username")) {
    std::cout << "User is logged in" << std::endl;
}
```

##### `void save_local_store() const`

**Description**: Persists the current database state to disk (dump.rdb file).

**Usage**:

```cpp
// Save database before shutdown
db->save_local_store();
```

##### `void read_local_store()`

**Description**: Loads database state from disk (dump.rdb file).

**Usage**:

```cpp
// Typically called automatically by constructor
db->read_local_store();
```

---

### Connection

The `Connection` class handles TCP networking, client connections, and communication using Boost.Asio.

#### Header Location

```cpp
#include "connection.h"
```

#### Class Definition

```cpp
class Connection {
public:
    Connection(boost::asio::io_context& io_context,
               const std::string& server_ip,
               int server_port,
               std::shared_ptr<handleCMD> handler_);
    ~Connection();
    bool start();
    void handle_client();
    int send_response(const std::string* response);
    size_t read(std::vector<uint8_t>&);
    bool is_connected() const;
    void disconnect();

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::acceptor acceptor_;
    int server_port_;
    std::string server_ip_;
    bool connected_;
    parser parser_;
    std::shared_ptr<handleCMD> handler_;
};
```

#### Methods

##### `Connection(boost::asio::io_context& io_context, const std::string& server_ip, int server_port, std::shared_ptr<handleCMD> handler_)`

**Description**: Constructor that initializes the connection with networking parameters.

**Parameters**:

- `io_context`: Boost.Asio I/O context for async operations
- `server_ip`: IP address to bind the server (string)
- `server_port`: Port number to listen on (int)
- `handler_`: Shared pointer to command handler

**Usage**:

```cpp
boost::asio::io_context io_context;
auto handler = std::make_shared<handleCMD>(db);
auto connection = std::make_shared<Connection>(
    io_context, "127.0.0.1", 6969, handler
);
```

##### `bool start()`

**Description**: Starts the server and begins accepting client connections.

**Returns**: `true` if server started successfully, `false` otherwise

**Usage**:

```cpp
if (!connection->start()) {
    std::cerr << "Failed to start server" << std::endl;
    return -1;
}
```

##### `void handle_client()`

**Description**: Main client handling loop that processes incoming commands.

**Usage**:

```cpp
connection->handle_client(); // Blocking call
```

##### `int send_response(const std::string* response)`

**Description**: Sends a response back to the connected client.

**Parameters**:

- `response`: Pointer to response string

**Returns**: Number of bytes sent

**Usage**:

```cpp
std::string response = "+OK\r\n";
connection->send_response(&response);
```

##### `bool is_connected() const`

**Description**: Checks if there's an active client connection.

**Returns**: `true` if connected, `false` otherwise

**Usage**:

```cpp
if (connection->is_connected()) {
    // Process commands
}
```

##### `void disconnect()`

**Description**: Closes the current client connection.

**Usage**:

```cpp
connection->disconnect();
```

---

### Parser

The `parser` class handles RESP (Redis Serialization Protocol) parsing for incoming commands.

#### Header Location

```cpp
#include "parser.h"
```

#### Class Definition

```cpp
class parser {
public:
    void addToBuffer(const std::vector<uint8_t>& data);
    bool isCommandValid();
    std::vector<std::string> parse();

private:
    std::vector<uint8_t> buffer_;
    std::string simpleStringParse(size_t&);
    std::string bulkStringParse(size_t&);
    std::vector<std::string> arrayParse(size_t&);
};
```

#### Methods

##### `void addToBuffer(const std::vector<uint8_t>& data)`

**Description**: Adds received data to the internal parsing buffer.

**Parameters**:

- `data`: Raw bytes received from client

**Usage**:

```cpp
std::vector<uint8_t> received_data;
// ... populate received_data from socket ...
parser_.addToBuffer(received_data);
```

##### `bool isCommandValid()`

**Description**: Checks if a complete command has been received.

**Returns**: `true` if complete command is ready for parsing

**Usage**:

```cpp
if (parser_.isCommandValid()) {
    auto command = parser_.parse();
    // Process command
}
```

##### `std::vector<std::string> parse()`

**Description**: Parses the buffered data into command components.

**Returns**: Vector of strings representing the parsed command and arguments

**Usage**:

```cpp
auto command_parts = parser_.parse();
// command_parts[0] = command name
// command_parts[1+] = arguments
```

---

### Command Handler

The `handleCMD` class dispatches commands to their respective implementations.

#### Header Location

```cpp
#include "handleCMD.h"
```

#### Class Definition

```cpp
class handleCMD {
public:
    handleCMD(std::shared_ptr<database> db);
    std::string handle(const std::vector<std::string>& input);
    std::shared_ptr<database> db_;

private:
    std::unordered_map<std::string, std::unique_ptr<command>> command_;
};
```

#### Methods

##### `handleCMD(std::shared_ptr<database> db)`

**Description**: Constructor that initializes the command handler with database reference.

**Parameters**:

- `db`: Shared pointer to database instance

**Usage**:

```cpp
auto db = std::make_shared<database>();
auto handler = std::make_shared<handleCMD>(db);
```

##### `std::string handle(const std::vector<std::string>& input)`

**Description**: Processes a command and returns the response.

**Parameters**:

- `input`: Vector containing command name and arguments

**Returns**: RESP-formatted response string

**Usage**:

```cpp
std::vector<std::string> command = {"SET", "key1", "value1"};
std::string response = handler->handle(command);
// response = "+OK\r\n"
```

---

## Command System

### Base Command Interface

All commands implement the `command` interface for consistent execution.

#### Header Location

```cpp
#include "commands/command.h"
```

#### Interface Definition

```cpp
class command {
public:
    virtual ~command() = default;
    virtual std::string execute(const std::vector<std::string> &args,
                                std::shared_ptr<database> db_) = 0;
};
```

#### Method

##### `virtual std::string execute(const std::vector<std::string> &args, std::shared_ptr<database> db_)`

**Description**: Executes the command with given arguments.

**Parameters**:

- `args`: Command arguments (args[0] is command name)
- `db_`: Database instance

**Returns**: RESP-formatted response

---

### Available Commands

#### PING Command

**Purpose**: Test server connectivity
**Usage**: `PING`
**Response**: `+PONG\r\n`

**Example**:

```bash
redis-cli -p 6969 PING
# Output: PONG
```

#### ECHO Command

**Purpose**: Echo back a message
**Usage**: `ECHO <message>`
**Response**: `+<message>\r\n`

**Example**:

```bash
redis-cli -p 6969 ECHO "Hello World"
# Output: Hello World
```

#### SET Command

**Purpose**: Set a key-value pair
**Usage**: `SET <key> <value>`
**Response**: `+OK\r\n` on success

**Example**:

```bash
redis-cli -p 6969 SET username "john_doe"
# Output: OK
```

#### GET Command

**Purpose**: Retrieve value by key
**Usage**: `GET <key>`
**Response**: `+<value>\r\n` if found, error if not found

**Example**:

```bash
redis-cli -p 6969 GET username
# Output: john_doe
```

#### DEL Command

**Purpose**: Delete a key
**Usage**: `DEL <key>`
**Response**: `:1\r\n` if deleted, `:0\r\n` if key didn't exist

**Example**:

```bash
redis-cli -p 6969 DEL username
# Output: (integer) 1
```

#### EXISTS Command

**Purpose**: Check if a key exists
**Usage**: `EXISTS <key>`
**Response**: `:1\r\n` if exists, `:0\r\n` if doesn't exist

**Example**:

```bash
redis-cli -p 6969 EXISTS username
# Output: (integer) 1
```

---

## Usage Examples

### Basic Server Setup

```cpp
#include <iostream>
#include "connection.h"
#include "database.h"
#include "handleCMD.h"
#include <boost/asio.hpp>

int main() {
    // Configuration
    std::string server_ip = "127.0.0.1";
    int server_port = 6969;

    // Initialize components
    boost::asio::io_context io_context;
    auto db = std::make_shared<database>();
    auto handler = std::make_shared<handleCMD>(db);

    // Create and start server
    auto connection = std::make_shared<Connection>(
        io_context, server_ip, server_port, handler
    );

    if (!connection->start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    std::cout << "Cedis server running on " << server_ip
              << ":" << server_port << std::endl;

    // Handle clients (blocking)
    connection->handle_client();

    // Save data before shutdown
    db->save_local_store();

    return 0;
}
```

### Custom Command Implementation

```cpp
#include "commands/command.h"

class customCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        if (args.size() != 2) {
            return "-ERR wrong number of arguments\r\n";
        }

        // Custom logic here
        std::string result = "Custom result for: " + args[1];
        return "+" + result + "\r\n";
    }
};
```

### Client Connection Example

```cpp
// Using redis-cli
redis-cli -h 127.0.0.1 -p 6969

// Basic operations
SET mykey "myvalue"
GET mykey
EXISTS mykey
DEL mykey
PING
ECHO "Hello Cedis"
```

---

## Error Handling

### Common Error Responses

- **Wrong number of arguments**: `-ERR wrong number of arguments\r\n`
- **Key not found**: Throws `std::runtime_error` in C++, returns error in RESP
- **Connection failures**: Boolean return values indicate success/failure

### Exception Safety

- Database operations are exception-safe
- Connection errors are handled gracefully
- RAII principles ensure proper resource cleanup

---

## Configuration

### Default Settings

```cpp
// Default server configuration
std::string server_ip = "127.0.0.1";
int server_port = 6969;

// Default persistence file
std::string dump_file = "dump.rdb";
```

### Build Requirements

```cmake
# CMakeLists.txt requirements
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)

# Required dependencies
find_package(Boost REQUIRED COMPONENTS system thread)
target_link_libraries(Cedis Boost::system Boost::thread)
```

### Compilation

```bash
# Build the project
mkdir build && cd build
cmake ..
make

# Run the server
./Cedis
```

---

## Thread Safety Notes

- The current implementation is designed for single-threaded use
- Database operations are not thread-safe without external synchronization
- Connection handling is single-client at a time
- For multi-threaded use, add appropriate locking mechanisms

---

## Performance Considerations

- **Memory Usage**: All data stored in memory (std::unordered_map)
- **Persistence**: Simple file-based persistence (not optimized for large datasets)
- **Networking**: Single-threaded event loop using Boost.Asio
- **Command Processing**: O(1) command lookup using hash map

---

## Future Extensions

### Adding New Commands

1. Create a new command class inheriting from `command`
2. Implement the `execute` method
3. Register the command in `handleCMD` constructor
4. Add appropriate includes in `handleCMD.h`

### Example:

```cpp
// In include/commands/incrCommand.h
class incrCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        if (args.size() != 2) return "-ERR wrong number of arguments\r\n";

        try {
            std::string current = db_->get(args[1]);
            int value = std::stoi(current) + 1;
            db_->set(args[1], std::to_string(value));
            return ":" + std::to_string(value) + "\r\n";
        } catch (...) {
            return "-ERR value is not an integer\r\n";
        }
    }
};
```
