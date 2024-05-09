#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

#include <UDPReceiver.h>

UDPReceiver::UDPReceiver(int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        throw std::runtime_error("Failed to create socket");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind socket");
}

UDPReceiver::~UDPReceiver()
{
    close(sockfd);
}

std::pair<std::string, Vector<double>> UDPReceiver::receive()
{
    socklen_t len = sizeof(addr);
    int read = recvfrom(sockfd, buffer, bufferSize, MSG_DONTWAIT, (sockaddr *)&addr, &len);

    if (read != bufferSize)
        throw std::runtime_error("Failed to receive position");

    return {std::string(inet_ntoa(addr.sin_addr)) + std::to_string(addr.sin_port), {buffer[0], buffer[1]}};
}