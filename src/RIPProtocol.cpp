// src/RIPProtocol.cpp
#include "../include/RIPProtocol.h"
#include "../include/Router.h"
#include <iostream>

RIPProtocol::RIPProtocol(Router* routerPtr) : router(routerPtr) {}

void RIPProtocol::addNeighbor(const std::string& neighborIP, const std::string& interface) {
    neighbors[neighborIP] = interface;
    std::cout << "[RIP] Added neighbor " << neighborIP << " on interface " << interface << "\n";
}

void RIPProtocol::receiveRouteUpdate(const std::string& fromNeighborIP,
    const std::vector<std::tuple<std::string, std::string, int>>& routes) {
    
    std::cout << "[RIP] Received route update from " << fromNeighborIP << ":\n";
    for (const auto& [network, subnet, metric] : routes) {
        int newMetric = metric + 1;
        std::cout << "  - Network: " << network << "/" << subnet
                  << ", Hop Count: " << newMetric << " via " << fromNeighborIP << "\n";
        // In a complete implementation, store this route in a RoutingTable with longest-prefix match
    }
}

void RIPProtocol::advertiseRoutes() const {
    std::cout << "[RIP] Advertising routes to neighbors...\n";
    for (const auto& [neighborIP, iface] : neighbors) {
        std::cout << "   -> Sent routing table to " << neighborIP << " via " << iface << "\n";
    }
}

void RIPProtocol::printNeighbors() const {
    std::cout << "\n[RIP] Neighbors:\n";
    for (const auto& [ip, iface] : neighbors) {
        std::cout << "  " << ip << " on " << iface << "\n";
    }
}

const std::unordered_map<std::string, std::string>& RIPProtocol::getNeighbors() const {
    return neighbors;
}
