// include/Switch.h
#ifndef SWITCH_H
#define SWITCH_H

#include <unordered_map>
#include <vector>
#include "Device.h"

class Switch {
private:
    std::vector<Device*> connectedDevices;
    std::unordered_map<std::string, Device*> macTable; // MAC → Device*

public:
    void connect(Device* device);
    void receiveFrame(const std::string& data, const std::string& srcMAC, const std::string& destMAC);
    void printMACTable() const;
};

#endif // SWITCH_H
