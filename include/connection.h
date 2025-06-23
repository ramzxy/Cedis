#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <boost/asio.hpp>
#include <memory>

/**
 * @class Connection
 * @brief Manages the network connection to the VPN server
 * 
 * The Connection class is responsible for:
 * 1. Establishing a secure TCP connection to the VPN server
 * 2. Authenticating with the server
 * 3. Maintaining the connection and handling reconnects
 * 4. Providing send/receive methods for encrypted data
 */
class Connection {
public:
    /**
     * @brief Constructor for Connection
     * @param io_context The Boost ASIO IO context for async operations
     * @param server_ip The IP address of the VPN server
     * @param server_port The port number of the VPN server
     * 
     * Initializes the connection but doesn't connect yet.
     */
    Connection(boost::asio::io_context& io_context, 
               const std::string& server_ip, 
               int server_port);
    
    /**
     * @brief Destructor - ensures clean disconnection
     */
    ~Connection();
    
    /**
     * @brief Connect to the VPN server
     * @return true if connection successful, false otherwise
     * 
     * This method:
     * 1. Resolves the server address
     * 2. Establishes a TCP connection
     * 3. Performs initial handshake
     */
    bool start();
    
    /**
     * @brief Disconnect from the VPN server
     * 
     * Sends a disconnect message and closes the socket.
     */
    void disconnect();
    
    /**
     * @brief Send data to the VPN server
     * @param data The data buffer to send
     * @param length The length of the data buffer
     * @return Number of bytes sent, or -1 on error
     * 
     * This method handles the low-level sending of data.
     * The data should already be encrypted before calling this.
     */
    int send_data(const uint8_t* data, size_t length);
    
    /**
     * @brief Receive data from the VPN server
     * @param data Buffer to store received data
     * @param max_length Maximum size of the buffer
     * @return Number of bytes received, or -1 on error
     * 
     * This method handles the low-level receiving of data.
     * The data will need to be decrypted after receiving.
     */
    int receive_data(uint8_t* data, size_t max_length);
    
    /**
     * @brief Check if the connection is active
     * @return true if connected, false otherwise
     */
    bool is_connected() const;
    
    /**
     * @brief Get the server IP address
     * @return The IP address of the VPN server
     */
    std::string server_ip_;
private:
    // Boost ASIO components for networking
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::acceptor acceptor_;
    
    // Server connection details
    int server_port_;
    
    // Connection state
    bool connected_;
    
};

#endif // CONNECTION_H 