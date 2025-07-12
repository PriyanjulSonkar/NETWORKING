// include/DHCP.h
#ifndef DHCP_H
#define DHCP_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Device.h"
#include "Router.h"

class DHCP {
private:
    std::vector<std::string> availableIPs;
    std::unordered_map<std::string, std::string> allocatedIPs; // deviceName → IP

public:
    DHCP();

    void requestIP(Device* device);
    void requestIP(Router* router, const std::string& interface);

    void printLeases() const;
};

#endif // DHCP_H
