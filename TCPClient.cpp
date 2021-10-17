#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;
using ip::tcp;

std::string getData(tcp::socket& socket) {
    streambuf buf;
    read_until(socket, buf, "\n");
    std::string data = buffer_cast<const char*> (buf.data());
    return data;
}
void sendData(tcp::socket& socket, const std::string& message) {
    write(socket, buffer(message + "\n"));
}

int main(int argc, char* argv[])
{
    io_service io_service;

    //socket creation

    ip::tcp::socket client_socket(io_service);
    client_socket
        .connect(
            tcp::endpoint(
                address::from_string("127.0.0.1"),
                9999));

    //getting username

    std::cout << "Enter your username: ";
    std::string name, reply, response;
    std::getline(std::cin, name);

    //sending username to another end and initiate the chat
    sendData(client_socket, name);

    //infite loop for the conversation
    while (true) {
        response = getData(client_socket);
        response.pop_back();

        if (response == "out") {
            std::cout << "Connection terminated" << std::endl;
            break;
        }
        std::cout << "Server: " << response << std::endl;
        std::cout << name << ":";
        std::getline(std::cin, reply);
        sendData(client_socket, reply);

        if (reply == "out")
            break;
    }
    return 0;
}
