// src/Switch.cpp
#include "../include/Switch.h"
#include <iostream>

void Switch::connect(Device* device) {
    connectedDevices.push_back(device);
    std::cout << "[SWITCH] Connected device: " << device->getName() << " (MAC: " << device->getMACAddress() << ")\n";
}

void Switch::receiveFrame(const std::string& data, const std::string& srcMAC, const std::string& destMAC) {
    std::cout << "[SWITCH] Frame received from MAC " << srcMAC << " destined for MAC " << destMAC << "\n";

    // Learn source MAC
    bool updated = (macTable[srcMAC] == nullptr);
    for (Device* d : connectedDevices) {
        if (d->getMACAddress() == srcMAC) {
            macTable[srcMAC] = d;
            if (updated)
                std::cout << "         → Learned: " << srcMAC << " is at port " << d->getName() << "\n";
            break;
        }
    }

    // Forward based on destination MAC
    if (macTable.find(destMAC) != macTable.end()) {
        Device* dest = macTable[destMAC];
        dest->setData(data);
        std::cout << "         → Forwarded to: " << dest->getName() << "\n";
    } else {
        std::cout << "         → Unknown destination, broadcasting...\n";
        for (Device* d : connectedDevices) {
            if (d->getMACAddress() != srcMAC) {
                d->setData(data);
                std::cout << "           → Delivered to: " << d->getName() << "\n";
            }
        }
    }
}

void Switch::printMACTable() const {
    std::cout << "\n[SWITCH] MAC Table:\n";
    for (const auto& [mac, device] : macTable) {
        std::cout << "  " << mac << " -> " << device->getName() << "\n";
    }
    std::cout << std::endl;
}
