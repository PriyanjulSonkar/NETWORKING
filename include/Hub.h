// include/Hub.h
#ifndef HUB_H
#define HUB_H

#include <vector>
#include <string>
#include "Device.h"

class Hub {
private:
    std::string name;
    std::vector<Device*> connectedDevices;

public:
    Hub(const std::string& name);
    
    void connect(Device* device);
    void broadcast(const std::string& data, Device* sender);
    void printMACAddresses() const;

    std::string getName() const;
};

#endif // HUB_H
