# Cedis

Cedis is a small Redis-protocol server written in C++17. It was built to explore asynchronous TCP I/O, RESP parsing, command dispatch, and simple in-memory persistence.

It is an experimental implementation, not a Redis replacement.

## What is implemented

- asynchronous accept, read, and write operations with Boost.Asio
- one shared in-memory key-value store across connected clients
- a subset of RESP2 arrays, bulk strings, and simple strings
- six commands: `PING`, `ECHO`, `SET`, `GET`, `DEL`, and `EXISTS`
- a plain-text snapshot written to `dump.rdb` during a clean shutdown

The snapshot file is Cedis-specific despite its name; it is not compatible with the Redis RDB format.

## Request path

```text
TCP client
    │
    ▼
server / Connection
    │
    ▼
RESP parser
    │
    ▼
command dispatcher
    │
    ▼
command implementation
    │
    ▼
shared database
```

`server` owns the asynchronous accept loop. Each accepted socket is handled by a `Connection`, which buffers incoming bytes and passes complete RESP messages to `parser`. `handleCMD` resolves the command name and invokes the corresponding command object against the shared `database`.

## Commands

| Command | Arguments | Response |
| --- | --- | --- |
| `PING` | none | simple string |
| `ECHO` | message | bulk string |
| `SET` | key, value | simple string |
| `GET` | key | bulk string or null bulk string |
| `DEL` | key | integer |
| `EXISTS` | key | integer |

`QUIT` is handled by the connection layer and closes the client socket.

## Build

Requirements:

- CMake 3.20 or newer
- a C++17 compiler
- Boost.System and Boost.Thread

```bash
git clone https://github.com/ramzxy/Cedis.git
cd Cedis

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The repository also contains `install_dependencies.sh` and `build.sh` for local setup.

## Run

```bash
./build/Cedis
```

The server listens on TCP port `6969`.

```bash
redis-cli -p 6969 PING
redis-cli -p 6969 SET answer 42
redis-cli -p 6969 GET answer
redis-cli -p 6969 EXISTS answer
redis-cli -p 6969 DEL answer
```

## Layout

```text
include/
  commands/       command implementations
  connection.h    per-client asynchronous I/O
  database.h      in-memory store and snapshot interface
  handleCMD.h     command registry and dispatcher
  parser.h        RESP parser
  server.h        accept loop
src/
  connection.cpp
  database.cpp
  handleCMD.cpp
  main.cpp
  parser.cpp
```

## Current limitations

- only a small RESP2 and command subset is implemented
- the server runs one `io_context` on one thread
- persistence is a tab-separated text snapshot and does not preserve every possible key or value
- there is no authentication, expiration, replication, clustering, or transaction support
- there is no automated unit or protocol-conformance test suite
- no performance claims are made; the project has not been benchmarked against Redis

