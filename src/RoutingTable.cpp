#include "../include/RoutingTable.h"
#include <bitset>
#include <sstream>
#include <iostream>

void RoutingTable::addRoute(const std::string& network, const std::string& subnetMask,
                            const std::string& nextHop, int metric, const std::string& interface) {
    RouteEntry entry{ network, subnetMask, nextHop, metric, interface };
    routes.push_back(entry);
}

std::pair<std::string, std::string> RoutingTable::findBestRoute(const std::string& targetIP) const {
    std::pair<std::string, std::string> bestRoute = { "", "" };
    int longestPrefix = -1;

    for (const auto& route : routes) {
        if (ipInNetwork(targetIP, route.network, route.subnetMask)) {
            // Count how many bits are set in the mask
            int maskBits = 0;
            std::stringstream maskStream(route.subnetMask);
            std::string octet;
            while (std::getline(maskStream, octet, '.')) {
                maskBits += std::bitset<8>(std::stoi(octet)).count();
            }

            if (maskBits > longestPrefix) {
                longestPrefix = maskBits;
                bestRoute = { route.nextHop, route.interface };
            }
        }
    }

    return bestRoute;
}

// Converts IP string to 32-bit binary
static std::bitset<32> ipToBinary(const std::string& ipStr) {
    std::stringstream ss(ipStr);
    std::string segment;
    std::bitset<32> bits;
    int shift = 24;

    while (std::getline(ss, segment, '.')) {
        bits |= (std::bitset<32>(std::stoi(segment)) << shift);
        shift -= 8;
    }
    return bits;
}

bool ipInNetwork(const std::string& ip, const std::string& network, const std::string& subnetMask) {
    std::bitset<32> ipBits = ipToBinary(ip);
    std::bitset<32> netBits = ipToBinary(network);
    std::bitset<32> maskBits = ipToBinary(subnetMask);

    return (ipBits & maskBits) == (netBits & maskBits);
}
