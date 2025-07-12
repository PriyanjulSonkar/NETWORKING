// src/TransportLayer.cpp
#include "../include/TransportLayer.h"
#include "../include/GoBackN.h"
#include "../include/CRC.h"
#include <iostream>

TransportLayer::TransportLayer() {
    goBackN = new GoBackN(4); // window size
}

void TransportLayer::openPort(Device* device, int port) {
    portTable[device->getName()] = port;
    std::cout << "[TRANSPORT] Opened port " << port << " on " << device->getName() << "\n";
}

int TransportLayer::getPort(Device* device) const {
    auto it = portTable.find(device->getName());
    if (it != portTable.end()) return it->second;
    return -1;
}

void TransportLayer::sendData(Device* sender, Device* receiver, const std::string& data) {
    std::string generator = "1001";
    std::string binary = CRC::textToBinary(data);
    std::string encoded = CRC::encodeData(binary, generator);

    std::cout << "[TRANSPORT] Sending from " << sender->getName() << " to " << receiver->getName() << "\n";
    std::cout << " -> Encoded Binary (with CRC): " << encoded << "\n";

    receiver->setData(encoded);
}


void TransportLayer::receiveACK(int ackNum) {
    goBackN->receiveACK(ackNum);
}

void TransportLayer::receiveNAK(int nakNum) {
    goBackN->receiveNAK(nakNum);
}
