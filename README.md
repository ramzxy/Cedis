# Cedis 🧠⚡ — Redis Reimagined in C++

**Cedis** is a high-performance, lightweight in-memory key-value store inspired by [Redis](https://redis.io/) and built from scratch in modern C++17 using Boost.Asio for asynchronous networking.

## ✨ Features

- 🔌 **Asynchronous Networking** - Built with Boost.Asio for efficient non-blocking I/O operations
- 🚀 **Multi-Client Support** - Handles multiple concurrent client connections
- 🧠 **RESP Protocol Support** - Compatible with Redis Serialization Protocol
- ⚙️ **Command Parser and Dispatcher** - Modular command processing with 6 implemented commands
- 🗃️ **In-Memory Key-Value Store** - Fast data storage using STL containers
- ♻️ **Modular Architecture** - Clean separation of concerns with command pattern
- 💻 **Cross-Platform Build System** - CMake-based build configuration with automated scripts
- 🖥️ **Multi-Platform Support** - Windows, macOS, and Linux compatibility
- 🔄 **Automated CI/CD** - GitHub Actions pipeline with cross-platform builds and releases
- 🛠️ **Professional Build Scripts** - One-command setup and build process

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

### 🚀 Quick Start (Recommended)

```bash
# Clone the repository
git clone https://github.com/ramzxy/cedis.git
cd cedis

# Install dependencies and build (all platforms)
./install_dependencies.sh
./build.sh

# Run the server
./build/Cedis
```

### 📦 Automated Build Scripts

#### **`install_dependencies.sh`** - Dependency Management

```bash
# Install dependencies for your platform
./install_dependencies.sh

# Verify dependencies are installed
./install_dependencies.sh --verify

# Show help
./install_dependencies.sh --help
```

**Features:**

- ✅ **Cross-platform support** - Linux, macOS, Windows
- ✅ **Automatic detection** - Detects your OS and package manager
- ✅ **Minimal dependencies** - Only installs what's needed
- ✅ **Error handling** - Comprehensive error checking and logging
- ✅ **CI/CD ready** - Works in GitHub Actions and other CI environments

#### **`build.sh`** - Build Automation

```bash
# Build in Release mode (default)
./build.sh

# Build in Debug mode
./build.sh --debug

# Build in Release mode (explicit)
./build.sh --release

# Clean build directory
./build.sh --clean

# Show help
./build.sh --help
```

**Features:**

- ✅ **Parallel builds** - Uses all available CPU cores
- ✅ **Multiple configurations** - Release and Debug builds
- ✅ **Project validation** - Checks for required files
- ✅ **Artifact verification** - Ensures build was successful
- ✅ **Professional logging** - Clear build output with timestamps

### 🔧 Manual Build Instructions

#### **Linux/macOS:**

```bash
# Clone and setup
git clone https://github.com/ramzxy/cedis.git
cd cedis

# Install dependencies
./install_dependencies.sh

# Build manually
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./Cedis
```

#### **Windows (Visual Studio):**

```cmd
# Clone and setup
git clone https://github.com/ramzxy/cedis.git
cd cedis

# Install dependencies (using vcpkg)
vcpkg install boost-system boost-thread

# Build manually
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release

# Run
.\build\Release\Cedis.exe
```

### 📋 Platform-Specific Dependencies

#### **Ubuntu/Debian:**

```bash
sudo apt update && sudo apt install libboost-system-dev libboost-thread-dev cmake build-essential
```

#### **CentOS/RHEL/Fedora:**

```bash
sudo dnf install boost-devel cmake gcc-c++
```

#### **macOS (Homebrew):**

```bash
brew install boost cmake
```

#### **Windows (vcpkg):**

```cmd
vcpkg install boost-system boost-thread
```

## 🔄 CI/CD Pipeline

Cedis includes a comprehensive GitHub Actions CI/CD pipeline that provides:

### **🏗️ Automated Builds**

- ✅ **Linux builds** - Ubuntu with Release/Debug configurations
- ✅ **macOS builds** - Latest macOS with Release/Debug configurations
- ✅ **Cross-platform testing** - Ensures compatibility across platforms

### **🧪 Quality Assurance**

- ✅ **Static analysis** - Clang-tidy code quality checks
- ✅ **Integration tests** - Automated server connectivity testing
- ✅ **Build verification** - Ensures artifacts are properly created
- ✅ **Dependency verification** - Validates all dependencies are available

### **🚀 Automated Releases**

- ✅ **Conditional releases** - Only creates releases with `[release]` commit messages
- ✅ **Cross-platform artifacts** - Linux and macOS binaries included
- ✅ **Release notes** - Automatic generation with features and installation instructions
- ✅ **Duplicate protection** - Prevents creating duplicate releases

### **📊 Pipeline Features**

```yaml
# Trigger conditions
on:
  push:
    branches: [main, develop]
  pull_request:
    branches: [main, develop]

# Jobs include:
- Linux builds (Release/Debug)
- macOS builds (Release/Debug)
- Code quality checks
- Integration tests
- Automated releases (with [release] keyword)
```

## 🚀 Getting Started

### Starting the Server

```bash
# Using build script (recommended)
./build.sh
./build/Cedis

# Or manually
mkdir build && cd build
cmake ..
make
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
├── scripts/
│   ├── install_dependencies.sh # Cross-platform dependency installer
│   └── build.sh               # Automated build script
├── .github/
│   └── workflows/
│       └── ci.yml             # CI/CD pipeline configuration
├── CMakeLists.txt             # Build configuration
├── API_DOCUMENTATION.md       # Comprehensive API documentation
├── QUICK_REFERENCE.md         # Quick command reference
└── README.md                  # Project documentation
```

## 🧪 Testing

### Automated Testing

- ✅ **CI/CD Integration Tests** - Automated server connectivity testing
- ✅ **Static Analysis** - Clang-tidy code quality checks
- ✅ **Cross-platform Build Tests** - Ensures builds work on all platforms

### Manual Testing

Currently, testing is performed manually using TCP clients. Future plans include:

- Unit tests for individual components
- Performance benchmarks
- Extended integration test suite

## 🤝 Contributing

We welcome contributions! Here's how to get started:

### Development Process

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/new-command`
3. **Install dependencies**: `./install_dependencies.sh`
4. **Implement** your changes with proper documentation
5. **Test** your changes: `./build.sh --debug`
6. **Commit** with clear messages: `git commit -m "Add EXPIRE command support"`
7. **Push** to your fork: `git push origin feature/new-command`
8. **Submit** a Pull Request with a detailed description

### Reporting Issues

Please use the GitHub issue tracker to report bugs or suggest features. Include:

- Operating system and version
- Compiler version
- Steps to reproduce the issue
- Expected vs. actual behavior

## 📋 Development Roadmap

### Phase 1: Core Functionality ✅

- [x] Basic RESP protocol parsing
- [x] Essential Redis commands (PING, SET, GET, DEL, EXISTS, ECHO)
- [x] Async client connection handling
- [x] Multi-client support with async I/O
- [x] Thread-safe database operations
- [x] Cross-platform build system
- [x] Automated CI/CD pipeline
- [x] Professional build scripts

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
- **Build Speed**: Optimized with parallel builds

## 🧑‍💻 Author

**Cedis** was developed by [Rmxzy](https://rmxzy.com) - a computer science student passionate about systems programming, networking, and high-performance computing.

## 🙏 Acknowledgments

- [Redis](https://redis.io/) - For inspiration and protocol specification
- [Boost.Asio](https://www.boost.org/doc/libs/1_82_0/doc/html/boost_asio.html) - For asynchronous networking capabilities
- The open-source community for tools and libraries

---

**⭐ Star this project** if you find it interesting or useful!

---

_Built with modern C++17, Boost.Asio, and professional CI/CD practices_
