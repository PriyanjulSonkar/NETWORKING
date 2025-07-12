// src/Router.cpp
#include "../include/Router.h"
#include "../include/RIPProtocol.h"
#include <iostream>

Router::Router(const std::string& routerName, NetworkLayer* layer)
    : Device(routerName),
      networkLayer(layer),
      rip(this) {}


Router::~Router() {
    
}

void Router::addInterface(const std::string& interfaceName, const std::string& ip, const std::string& mask) {
    interfaces[interfaceName] = {ip, mask};
    std::cout << "[ROUTER] " << getName() << ": Interface " << interfaceName << " -> " << ip << "/" << mask << "\n";
}

void Router::addStaticRoute(const std::string& network, const std::string& mask, const std::string& nextHop) {
    std::cout << "[ROUTER] " << getName() << ": Static route added → " << network << "/" << mask
              << " via " << nextHop << "\n";
    // In a full impl, you'd store and lookup this route (handled by RIPProtocol or RoutingTable)
}

void Router::receiveARPReply(const std::string& ip, const std::string& mac) {
    arpCache[ip] = mac;
    std::cout << "[ARP] " << getName() << " updated ARP table: " << ip << " -> " << mac << "\n";
}

void Router::sendARPRequest(const std::string& targetIP) const {
    std::cout << "[ARP] " << getName() << " sent ARP request for " << targetIP << "\n";
}

void Router::forwardPacket(const Packet& packet) {
    std::cout << "[ROUTER] " << getName() << " forwarding packet to: " << packet.getDestinationIP() << "\n";
    if (arpCache.find(packet.getDestinationIP()) == arpCache.end()) {
        sendARPRequest(packet.getDestinationIP());
        return;
    }

    std::string destMAC = arpCache.at(packet.getDestinationIP());
    std::cout << "  → Resolved MAC: " << destMAC << "\n";
    std::cout << "  → Payload: " << packet.getPayload() << "\n";
}


void Router::printInterfaces() const {
    std::cout << "\n[ROUTER] " << getName() << " Interfaces:\n";
    for (const auto& [iface, config] : interfaces) {
        std::cout << "  " << iface << " -> " << config.first << "/" << config.second << "\n";
    }
}

void Router::printARPTable() const {
    std::cout << "\n[ROUTER] " << getName() << " ARP Table:\n";
    for (const auto& [ip, mac] : arpCache) {
        std::cout << "  " << ip << " -> " << mac << "\n";
    }
}


std::string Router::getName() const {
    return name;
}


RIPProtocol* Router::getRIP() {
    return &rip;
}
