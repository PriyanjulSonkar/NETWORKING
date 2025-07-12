// src/Bridge.cpp
#include "../include/Bridge.h"
#include <iostream>

void Bridge::connectToSegment(Device* device, int segment) {
    if (segment == 1) {
        segment1.push_back(device);
        std::cout << "[BRIDGE] Connected " << device->getName() << " to Segment 1\n";
    } else if (segment == 2) {
        segment2.push_back(device);
        std::cout << "[BRIDGE] Connected " << device->getName() << " to Segment 2\n";
    } else {
        std::cout << "[BRIDGE] Invalid segment!\n";
    }
}

void Bridge::receiveFrame(const std::string& data, const std::string& srcMAC, const std::string& destMAC) {
    std::cout << "[BRIDGE] Frame received from MAC " << srcMAC << " to MAC " << destMAC << "\n";

    // Learn source MAC
    int sourceSegment = -1;
    for (Device* d : segment1) {
        if (d->getMACAddress() == srcMAC) {
            sourceSegment = 1;
            break;
        }
    }
    for (Device* d : segment2) {
        if (d->getMACAddress() == srcMAC) {
            sourceSegment = 2;
            break;
        }
    }
    if (sourceSegment != -1) {
        macTable[srcMAC] = sourceSegment;
        std::cout << "         → Learned: " << srcMAC << " is on Segment " << sourceSegment << "\n";
    }

    // Decide destination segment
    int destSegment = (macTable.find(destMAC) != macTable.end()) ? macTable[destMAC] : 0;

    if (destSegment == 0 || destSegment == sourceSegment) {
        std::cout << "         → Destination unknown or same segment. Broadcasting...\n";
        std::vector<Device*>& targetSegment = (sourceSegment == 1) ? segment2 : segment1;
        for (Device* d : targetSegment) {
            d->setData(data);
            std::cout << "           → Delivered to: " << d->getName() << "\n";
        }
    } else {
        std::vector<Device*>& targetSegment = (destSegment == 1) ? segment1 : segment2;
        for (Device* d : targetSegment) {
            if (d->getMACAddress() == destMAC) {
                d->setData(data);
                std::cout << "         → Forwarded to: " << d->getName() << " on Segment " << destSegment << "\n";
            }
        }
    }
}

void Bridge::printMACTable() const {
    std::cout << "\n[BRIDGE] MAC Table:\n";
    for (const auto& entry : macTable) {
        std::cout << "  " << entry.first << " → Segment " << entry.second << "\n";
    }
    std::cout << std::endl;
}
