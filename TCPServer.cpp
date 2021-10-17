#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using namespace boost::asio::ip;

std::string getData(tcp::socket& socket) {
    streambuf buf;
    read_until(socket, buf, "\n");
    std::string data = buffer_cast<const char*>(buf.data());
    return data;
}

//driver program to send data
void sendData(tcp::socket& socket, const std::string& message) {
    write(socket, buffer(message + "\n"));
}

int main(int argc, char* argv[]) 
{
    io_service io_service;

    //new incoming connection at port 9999 with ipv4 protocol
    tcp::acceptor acceptor_server(
        io_service,
        tcp::endpoint(tcp::v4(), 9999));

    //creating socket object
    tcp::socket server_socket(io_service);

    //waiting for connection
    acceptor_server.accept(server_socket);

    //reading username
    std::string name = getData(server_socket);

    //removing "\n" from the username
    name.pop_back();

    //replying with default message to initiate chat

    std::string response, reply;
    reply = "Hello " + name + "!";
    std::cout << "Server: " << reply << std::endl;
    sendData(server_socket, reply);

    while (true) {
        //fetching response
        response = getData(server_socket);

        //displaying the last character
        response.pop_back();

        //validating if the connection has to be closed
        if (response == "out") {
            std::cout << name << "has just left!" << std::endl;
            break;
        }
        std::cout << name<< ": " << response << std::endl;

        //reading new message from input stream
        std::cout << "Server" << ": ";
        std::getline(std::cin, reply);
        sendData(server_socket, reply);

        if (reply == "out")
            break;
    }
    return 0;   
}
