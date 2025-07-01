# Cedis 🧠⚡ — Redis Reimagined in C++

Cedis is a high-performance, lightweight in-memory key-value store, inspired by [Redis](https://redis.io/) and built from scratch in modern C++17 using Boost.Asio for asynchronous networking.




## ✨ Features

- 🔌 **Asynchronous Networking** via Boost.Asio  
- 🧠 **RESP Protocol** (Redis Serialization Protocol) support  
- ⚙️ **Command Parser and Dispatcher** (GET, SET, PING, etc.)  
- 🗃️ **In-Memory Key-Value Store**  
- ♻️ **Modular Class Design**  
- 💻 **Cross-platform Build System with CMake**  
- ✅ **Ready for GitHub Actions CI**  




## 🏗️ Building the Project

### 🔹 Prerequisites

- C++17 compiler (GCC, Clang, MSVC)  
- [Boost](https://www.boost.org/) (specifically `boost::asio` and `boost::system`)  
- [CMake](https://cmake.org/) ≥ 3.16  

### 🔹Build manually

```bash
cmake -S . -B build
cmake --build build
./build/bin/cedis
```


## 🚀 Getting Started

Once compiled, run the server:

```bash
./cedis
```

Then use redis-cli or any TCP client:

```bash
redis-cli -p 6379
```

Try commands like:

```bash
PING
SET hello world
GET hello
```

## 🧱 Architecture Overview

Cedis is built using a modular design:

- RedisServer: Accepts client connections

- Connection: Handles socket I/O per client
    
- Parser: Parses RESP input from clients

- Command: Abstract class for supported commands

- CommandFactory: Instantiates command objects

- DataStore: In-memory, thread-safe key-value map

- Each Connection runs asynchronously and interacts with shared components like DataStore to execute commands.

## 🤝 Contributing

Contributions, feature suggestions, and pull requests are welcome! Just fork the repo and open a PR.

To suggest features or report bugs, open an issue.
## 📘 Future Plans

- Add more Redis commands (DEL, EXPIRE, etc.)

- Add RESP error handling

- Implement persistence (RDB or AOF)

- Add unit tests (GoogleTest or Catch2)

- Add Dockerfile for easy deployment

## 🧑‍💻 Author

Cedis was built by [Rmxzy](rmxzy.com), a computer science student exploring systems-level programming, networking, and performance.

If you like this, feel free to ⭐ the repo or connect!
