// src/ARP.cpp
#include "../include/ARP.h"

void ARP::cacheEntry(const std::string& ip, const std::string& mac) {
    table[ip] = mac;
    std::cout << "[ARP] Cached: " << ip << " -> " << mac << "\n";
}

std::string ARP::resolve(const std::string& ip) const {
    auto it = table.find(ip);
    if (it != table.end()) {
        std::cout << "[ARP] Resolved " << ip << " to " << it->second << "\n";
        return it->second;
    } else {
        std::cout << "[ARP] IP " << ip << " not found in ARP cache.\n";
        return "00:00:00:00:00:00";
    }
}

void ARP::printTable() const {
    std::cout << "\n[ARP] ARP Table:\n";
    for (const auto& [ip, mac] : table) {
        std::cout << "  " << ip << " -> " << mac << "\n";
    }
}
