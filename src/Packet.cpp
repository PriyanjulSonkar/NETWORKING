// src/Packet.cpp
#include "../include/Packet.h"
#include <iostream>

Packet::Packet(const std::string& srcIP, const std::string& dstIP, const std::string& data)
    : sourceIP(srcIP), destinationIP(dstIP), payload(data) {}

std::string Packet::getSourceIP() const {
    return sourceIP;
}

std::string Packet::getDestinationIP() const {
    return destinationIP;
}

std::string Packet::getPayload() const {
    return payload;
}

void Packet::printHeader() const {
    std::cout << "[NETWORK] Packet Header:\n";
    std::cout << "  Source IP:      " << sourceIP << "\n";
    std::cout << "  Destination IP: " << destinationIP << "\n";
    std::cout << "  Payload:        " << payload << "\n";
}
