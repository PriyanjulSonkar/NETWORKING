// include/Bridge.h
#ifndef BRIDGE_H
#define BRIDGE_H

#include "Device.h"
#include <unordered_map>
#include <vector>
#include <string>

class Bridge {
private:
    std::vector<Device*> segment1;
    std::vector<Device*> segment2;
    std::unordered_map<std::string, int> macTable; // MAC → segment (1 or 2)

public:
    void connectToSegment(Device* device, int segment); // segment = 1 or 2
    void receiveFrame(const std::string& data, const std::string& srcMAC, const std::string& destMAC);
    void printMACTable() const;
};

#endif // BRIDGE_H
