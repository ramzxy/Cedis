# Cedis Quick Reference Guide

## Essential APIs Cheat Sheet

### Database Operations

```cpp
// Include
#include "database.h"

// Initialize
auto db = std::make_shared<database>();

// Basic operations
db->set("key", "value");           // Store key-value
std::string val = db->get("key");  // Retrieve value (throws if not found)
bool deleted = db->del("key");     // Delete key (returns true if existed)
bool exists = db->exists("key");   // Check existence

// Persistence
db->save_local_store();            // Save to dump.rdb
db->read_local_store();            // Load from dump.rdb (auto-called in constructor)
```

### Server Setup

```cpp
// Includes
#include "connection.h"
#include "database.h"
#include "handleCMD.h"
#include <boost/asio.hpp>

// Quick server setup
boost::asio::io_context io_context;
auto db = std::make_shared<database>();
auto handler = std::make_shared<handleCMD>(db);
auto connection = std::make_shared<Connection>(io_context, "127.0.0.1", 6969, handler);

if (connection->start()) {
    connection->handle_client(); // Blocking
}
```

### Command Implementation Template

```cpp
// New command header: include/commands/myCommand.h
#ifndef MY_COMMAND_H
#define MY_COMMAND_H
#include "command.h"

class myCommand : public command {
public:
    std::string execute(const std::vector<std::string>& args,
                       std::shared_ptr<database> db_) override {
        // Validate arguments
        if (args.size() != expected_size) {
            return "-ERR wrong number of arguments\r\n";
        }

        // Your logic here
        // Use db_ for database operations

        // Return RESP format:
        // "+OK\r\n" for simple string
        // ":123\r\n" for integer
        // "-ERR message\r\n" for error
    }
};
#endif
```

## Redis-CLI Examples

```bash
# Connect to Cedis
redis-cli -h 127.0.0.1 -p 6969

# Test connectivity
PING                    # Returns: PONG

# Basic key-value operations
SET mykey "hello"       # Returns: OK
GET mykey              # Returns: hello
EXISTS mykey           # Returns: (integer) 1
DEL mykey              # Returns: (integer) 1

# Echo command
ECHO "Hello World"     # Returns: Hello World
```

## RESP Protocol Quick Reference

| Type          | Format                      | Example                            |
| ------------- | --------------------------- | ---------------------------------- |
| Simple String | `+<string>\r\n`             | `+OK\r\n`                          |
| Error         | `-<error>\r\n`              | `-ERR message\r\n`                 |
| Integer       | `:<number>\r\n`             | `:42\r\n`                          |
| Bulk String   | `$<length>\r\n<string>\r\n` | `$5\r\nhello\r\n`                  |
| Array         | `*<count>\r\n<elements>`    | `*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n` |

## Common Error Patterns

```cpp
// Wrong argument count
if (args.size() != expected) {
    return "-ERR wrong number of arguments\r\n";
}

// Key not found (in database.cpp, get() throws)
try {
    std::string value = db->get(key);
    return "+" + value + "\r\n";
} catch (const std::runtime_error&) {
    return "-ERR key not found\r\n";
}

// Invalid integer
try {
    int num = std::stoi(args[1]);
    // use num...
} catch (const std::exception&) {
    return "-ERR value is not an integer\r\n";
}
```

## Build Commands

```bash
# Standard build
mkdir build && cd build
cmake ..
make

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# With specific Boost path (if needed)
cmake -DBOOST_ROOT=/path/to/boost ..
make
```

## Dependencies Quick Install

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake libboost-all-dev
```

### macOS (Homebrew)

```bash
brew install cmake boost
```

### Windows (vcpkg)

```cmd
vcpkg install boost-asio boost-system boost-thread
```

## Connection Flow

1. **Initialize**: `database` → `handleCMD` → `Connection`
2. **Start**: `connection->start()` (binds to port)
3. **Accept**: `connection->handle_client()` (accepts first client)
4. **Process**: Command loop (read → parse → execute → respond)
5. **Cleanup**: Automatic RAII destruction + `db->save_local_store()`

## Debugging Tips

```cpp
// Add debug output in commands
std::cout << "Command: " << args[0] << " with " << args.size()-1 << " args" << std::endl;

// Check connection status
if (connection->is_connected()) {
    std::cout << "Client connected" << std::endl;
}

// Verify database state
std::cout << "Key exists: " << (db->exists(key) ? "yes" : "no") << std::endl;
```

## Memory Layout

```
Connection
├── Boost.Asio socket/acceptor
├── parser (RESP buffer)
└── handleCMD
    ├── command map<string, unique_ptr<command>>
    └── database (shared_ptr)
        └── std::unordered_map<string, string> store_
```
