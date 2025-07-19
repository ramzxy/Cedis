# Cedis 🧠⚡ — Redis Reimagined in C++

**Cedis** is a high-performance, lightweight in-memory key-value store inspired by [Redis](https://redis.io/) and built from scratch in modern C++17 using Boost.Asio for asynchronous networking.

## ✨ Features

- 🔌 **Asynchronous Networking** - Built with Boost.Asio for efficient non-blocking I/O operations
- 🚀 **Multi-Client Support** - Handles multiple concurrent client connections
- 🧠 **RESP Protocol Support** - Compatible with Redis Serialization Protocol
- ⚙️ **Command Parser and Dispatcher** - Modular command processing with 6 implemented commands
- 🗃️ **In-Memory Key-Value Store** - Fast data storage using STL containers
- ♻️ **Modular Architecture** - Clean separation of concerns with command pattern
- 💻 **Cross-Platform Build System** - CMake-based build configuration
- 🖥️ **Multi-Platform Support** - Windows, macOS, and Linux compatibility

## 🎯 Implemented Commands

Currently supported Redis-compatible commands:

| Command  | Description              | Usage               |
| -------- | ------------------------ | ------------------- |
| `PING`   | Test server connectivity | `PING`              |
| `ECHO`   | Echo back a message      | `ECHO <message>`    |
| `SET`    | Set a key-value pair     | `SET <key> <value>` |
| `GET`    | Retrieve value by key    | `GET <key>`         |
| `DEL`    | Delete a key             | `DEL <key>`         |
| `EXISTS` | Check if a key exists    | `EXISTS <key>`      |

## 🏗️ Building the Project

### Prerequisites

- **C++17 Compiler** - GCC 7+, Clang 5+, or MSVC 2017+
- **[Boost Libraries](https://www.boost.org/)** - Version 1.70+ (system and thread components only)
- **[CMake](https://cmake.org/)** - Version 3.20 or higher

### Why This Approach?

Cedis uses a **header-only approach** for Boost.Asio, which means:

- ✅ **No complex dependency management** - Just install Boost system and thread libraries
- ✅ **Cross-platform compatibility** - Works on any platform with Boost installed
- ✅ **Simple build process** - Clean CMakeLists.txt with minimal configuration
- ✅ **Easy to compile anywhere** - No platform-specific Boost detection needed

### Build Instructions

#### Quick Start (All Platforms)

```bash
# Clone the repository
git clone https://github.com/ramzxy/cedis.git
cd cedis

# Install dependencies (if needed)
./install_dependencies.sh

# Build the project
./build.sh

# Run the server
./build/Cedis
```

#### Manual Build

**Windows (Visual Studio):**

```cmd
# Clone the repository
git clone https://github.com/ramzxy/cedis.git
cd cedis

# Configure and build
cmake -S . -B build -G "Visual Studio 16 2019"
cmake --build build --config Release

# Run the executable
.\build\Release\Cedis.exe
```

**macOS/Linux:**

```bash
# Clone the repository
git clone https://github.com/ramzxy/cedis.git
cd cedis

# Configure and build
mkdir build && cd build
cmake ..
make

# Run the executable
./Cedis
```

#### Installing Boost Dependencies

**Ubuntu/Debian:**

```bash
sudo apt update && sudo apt install libboost-all-dev cmake build-essential
```

**CentOS/RHEL/Fedora:**

```bash
sudo dnf install boost-devel cmake gcc-c++
```

**macOS (Homebrew):**

```bash
brew install boost cmake
```

**Windows (vcpkg):**

```cmd
vcpkg install boost-system boost-thread
```

## 🚀 Getting Started

### Starting the Server

```bash
./Cedis
```

The server will start listening on `127.0.0.1:6969` (port 6969) (because im funny).

### Connecting to the Server

You can connect using any TCP client:

```bash
# Using netcat
nc 127.0.0.1 6969

# Using telnet
telnet 127.0.0.1 6969

# Using redis-cli (if available)
redis-cli -h 127.0.0.1 -p 6969
```

### Example Usage

Once connected, you can use Redis-compatible commands:

```bash
PING
# Response: +PONG

SET greeting "Hello, World!"
# Response: +OK

GET greeting
# Response: Hello, World!

EXISTS greeting
# Response: +OK

DEL greeting
# Response: +OK

ECHO "Hello from Cedis!"
# Response: Hello from Cedis!
```

### Multi-Client Testing

The server now supports multiple concurrent connections. You can test this by opening multiple terminals:

```bash
# Terminal 1
redis-cli -h 127.0.0.1 -p 6969
SET client1 "Hello from client 1"

# Terminal 2 (simultaneously)
redis-cli -h 127.0.0.1 -p 6969
SET client2 "Hello from client 2"
GET client1  # Should return the value set by client 1
```

### Async Features

- **Non-blocking I/O**: The server handles multiple clients without blocking
- **Automatic Connection Management**: Connections are cleaned up automatically when clients disconnect
- **Shared Database**: All clients share the same database instance for data consistency
- **Event-Driven**: Uses Boost.Asio's event loop for efficient resource usage

## 🧱 Architecture Overview

Cedis follows a modular architecture designed for maintainability and extensibility with asynchronous networking capabilities:

### Core Components

- **`server`** - Async server that accepts and manages multiple client connections
- **`Connection`** - Manages individual client socket connections using Boost.Asio async I/O
- **`parser`** - Parses RESP (Redis Serialization Protocol) messages
- **`command`** - Abstract base class defining the command interface
- **`handleCMD`** - Command dispatcher that routes requests to appropriate handlers
- **`database`** - In-memory key-value store using `std::unordered_map`
- **Command Classes** - Individual implementations (`pingCommand`, `setCommand`, etc.)

### Design Patterns

- **Command Pattern** - Each Redis command is encapsulated as a separate class
- **Factory Pattern** - Command dispatcher creates and executes appropriate command objects
- **RAII** - Automatic resource management with proper cleanup
- **Modern C++** - Leverages C++17 features and STL containers
- **Async/Await Pattern** - Uses Boost.Asio's async operations for non-blocking I/O

### Current Limitations

- **Basic Persistence** - Simple file-based persistence (dump.rdb)
- **Basic Error Handling** - Limited error recovery mechanisms
- **No Authentication** - No security mechanisms implemented

## 🔧 Project Structure

```
Cedis/
├── include/
│   ├── commands/              # Command implementations
│   │   ├── pingCommand.h      # PING command
│   │   ├── setCommand.h       # SET command
│   │   ├── getCommand.h       # GET command
│   │   ├── delCommand.h       # DEL command
│   │   ├── existsCommand.h    # EXISTS command
│   │   └── echoCommand.h      # ECHO command
│   ├── command.h              # Abstract command base class
│   ├── connection.h           # Async client connection handling
│   ├── server.h               # Async server with multi-client support
│   ├── database.h             # Key-value store interface
│   ├── handleCMD.h            # Command dispatcher
│   └── parser.h               # RESP protocol parser
├── src/
│   ├── connection.cpp         # Async connection implementation
│   ├── database.cpp           # Database operations
│   ├── handleCMD.cpp          # Command handling logic
│   ├── main.cpp               # Application entry point
│   └── parser.cpp             # RESP parsing logic
├── CMakeLists.txt             # Build configuration
└── README.md                  # Project documentation
```

## 🧪 Testing

Currently, testing is performed manually using TCP clients. Future plans include:

- Unit tests for individual components
- Integration tests for command processing
- Performance benchmarks
- Automated CI/CD pipeline

## 🤝 Contributing

We welcome contributions! Here's how to get started:

### Development Process

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/new-command`
3. **Implement** your changes with proper documentation
4. **Test** your changes thoroughly
5. **Commit** with clear messages: `git commit -m "Add EXPIRE command support"`
6. **Push** to your fork: `git push origin feature/new-command`
7. **Submit** a Pull Request with a detailed description

### Reporting Issues

Please use the GitHub issue tracker to report bugs or suggest features. Include:

- Operating system and version
- Compiler version
- Steps to reproduce the issue
- Expected vs. actual behavior

## 📋 Development Roadmap

### Phase 1: Core Functionality

- [x] Basic RESP protocol parsing
- [x] Essential Redis commands (PING, SET, GET, DEL, EXISTS, ECHO)
- [x] Async client connection handling
- [x] Multi-client support with async I/O
- [x] Thread-safe database operations

### Phase 2: Advanced Features

- [ ] **Additional Commands** - EXPIRE, TTL, INCR, DECR, APPEND, KEYS
- [ ] **Data Types** - Lists, Sets, Sorted Sets, Hashes
- [ ] **Persistence** - RDB snapshots and AOF logging
- [ ] **Configuration** - Config file support and runtime configuration
- [ ] **Memory Management** - Efficient memory usage and cleanup

### Phase 3: Production Readiness

- [ ] **Authentication** - AUTH command and user management
- [ ] **Replication** - Master-slave replication
- [ ] **Clustering** - Distributed deployment support
- [ ] **Monitoring** - Health checks and metrics
- [ ] **Performance** - Benchmarking and optimization

### Phase 4: Extended Features

- [ ] **Lua Scripting** - EVAL command support
- [ ] **Pub/Sub** - Publisher/Subscriber messaging
- [ ] **Transactions** - MULTI/EXEC command support
- [ ] **Modules** - Plugin architecture
- [ ] **Docker Support** - Containerization and orchestration

## 📈 Performance Characteristics

- **Memory Usage**: Minimal overhead with STL containers
- **Throughput**: Single-threaded, non-blocking async I/O
- **Latency**: Low latency for in-memory operations
- **Scalability**: Multi-client support with async connection handling
- **Concurrency**: Handles multiple concurrent client connections efficiently

## 🧑‍💻 Author

**Cedis** was developed by [Rmxzy](https://rmxzy.com) - a computer science student passionate about systems programming, networking, and high-performance computing.

## 🙏 Acknowledgments

- [Redis](https://redis.io/) - For inspiration and protocol specification
- [Boost.Asio](https://www.boost.org/doc/libs/1_82_0/doc/html/boost_asio.html) - For asynchronous networking capabilities
- The open-source community for tools and libraries

---

**⭐ Star this project** if you find it interesting or useful!

---

_Built with modern C++17 and Boost.Asio_
