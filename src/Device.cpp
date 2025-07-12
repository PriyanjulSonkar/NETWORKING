// src/Device.cpp
#include "../include/Device.h"
#include "../include/CRC.h"
#include <iostream>

Device::Device(std::string deviceName)
    : name(deviceName), ipAddress(""), macAddress(""), port(-1), dataBuffer("") {}

void Device::setIPAddress(const std::string& ip) {
    ipAddress = ip;
}

std::string Device::getIPAddress() const {
    return ipAddress;
}

void Device::setMACAddress(const std::string& mac) {
    macAddress = mac;
}

std::string Device::getMACAddress() const {
    return macAddress;
}

void Device::setPort(int portNumber) {
    port = portNumber;
}

int Device::getPort() const {
    return port;
}

void Device::setData(const std::string& receivedBinary) {
    std::string generator = "1001";
    std::cout << "[DEVICE] " << name << " received binary: " << receivedBinary << "\n";

    if (CRC::checkError(receivedBinary, generator)) {
        std::string pureBinary = CRC::extractOriginalMessage(receivedBinary, generator);
        std::string originalText = CRC::binaryToText(pureBinary);
        data = originalText;

        std::cout << "CRC Verified. Message Received: " << originalText << "\n";
    } else {
        data = "";
        std::cout << "CRC ERROR! Corrupted data.\n";
    }
}


std::string Device::getData() const {
    return dataBuffer;
}

std::string Device::getName() const {
    return name;
}
