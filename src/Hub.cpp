// src/Hub.cpp
#include "../include/Hub.h"
#include <iostream>

Hub::Hub(const std::string& name) : name(name) {}

void Hub::connect(Device* device) {
    connectedDevices.push_back(device);
    std::cout << "[PHYSICAL] " << device->getName() << " connected to " << name << "\n";
}

void Hub::broadcast(const std::string& data, Device* sender) {
    std::cout << "[PHYSICAL] Broadcasting from " << sender->getName() << ": " << data << "\n";
    for (Device* device : connectedDevices) {
        if (device != sender) {
            device->setData(data);
            std::cout << "  → Data received by " << device->getName() << "\n";
        }
    }
}

void Hub::printMACAddresses() const {
    std::cout << "\n[HUB] MAC Address Table:\n";
    for (Device* device : connectedDevices) {
        std::cout << "  - " << device->getName() << ": " << device->getMACAddress() << "\n";
    }
    std::cout << std::endl;
}

std::string Hub::getName() const {
    return name;
}
