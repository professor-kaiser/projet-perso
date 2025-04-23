#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::v4());
        
        udp::resolver resolver(io_context);
        udp::endpoint server_endpoint = *resolver.resolve(udp::v4(), "127.0.0.1", "12345").begin();

        std::string message = "Hello from client\n";
        socket.send_to(boost::asio::buffer(message), server_endpoint);

        std::array<char, 1024> recv_buffer;
        udp::endpoint sender_endpoint;
        size_t bytes_transferred = socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);

        std::string response(recv_buffer.data(), bytes_transferred);
        std::cout << "Server response: " << response << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}