// src/DHCP.cpp
#include "../include/DHCP.h"
#include <iostream>

DHCP::DHCP() {
    availableIPs = {
        "192.168.1.2", "192.168.1.3", "192.168.1.4",
        "192.168.1.5", "192.168.1.6", "192.168.1.7"
    };
}

void DHCP::requestIP(Device* device) {
    if (allocatedIPs.find(device->getName()) != allocatedIPs.end()) {
        std::cout << "[DHCP] " << device->getName() << " already has IP: "
                  << allocatedIPs[device->getName()] << "\n";
        return;
    }

    if (!availableIPs.empty()) {
        std::string ip = availableIPs.front();
        availableIPs.erase(availableIPs.begin());
        allocatedIPs[device->getName()] = ip;
        device->setIPAddress(ip);

        std::cout << "[DHCP] Assigned IP " << ip << " to " << device->getName() << "\n";
    } else {
        std::cout << "[DHCP] No available IPs for " << device->getName() << "\n";
    }
}

void DHCP::requestIP(Router* router, const std::string& interface) {
    if (!availableIPs.empty()) {
        std::string ip = availableIPs.front();
        availableIPs.erase(availableIPs.begin());

        router->addInterface(interface, ip, "255.255.255.0");
        std::string key = "router_" + interface;
        allocatedIPs[key] = ip;

        std::cout << "[DHCP] Assigned IP " << ip << " to " << router->getName()
                  << " interface " << interface << "\n";
    } else {
        std::cout << "[DHCP] No available IPs for router interface " << interface << "\n";
    }
}

void DHCP::printLeases() const {
    std::cout << "\n[DHCP] Current IP Leases:\n";
    for (const auto& lease : allocatedIPs) {
        std::cout << "  " << lease.first << " → " << lease.second << "\n";
    }
}
