// include/Packet.h
#ifndef PACKET_H
#define PACKET_H

#include <string>

class Packet {
private:
    std::string sourceIP;
    std::string destinationIP;
    std::string payload;

public:
    Packet(const std::string& srcIP, const std::string& dstIP, const std::string& data);

    std::string getSourceIP() const;
    std::string getDestinationIP() const;
    std::string getPayload() const;

    void printHeader() const;
};

#endif // PACKET_H
