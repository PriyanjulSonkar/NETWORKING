// include/Router.h
#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "Device.h"     
#include "Packet.h"
#include "RIPProtocol.h"
#include "NetworkLayer.h"
#include "RoutingTable.h"

struct Interface {
    std::string ipAddress;
    std::string subnetMask;
};

class Router : public Device {
private:
    std::string name;
    NetworkLayer* networkLayer;  // ✅ Add this line
    RoutingTable routingTable;
    RIPProtocol rip;
    std::map<std::string, std::pair<std::string, std::string>> interfaces;
    std::map<std::string, std::string> arpCache;

public:
    Router(const std::string& name, NetworkLayer* layer = nullptr);
    ~Router();

    void addInterface(const std::string& name, const std::string& ip, const std::string& mask);
    void addStaticRoute(const std::string& network, const std::string& mask, const std::string& nextHop);

    void receiveARPReply(const std::string& ip, const std::string& mac);
    void sendARPRequest(const std::string& targetIP) const;

    void forwardPacket(const Packet& packet);
    void printInterfaces() const;
    void printARPTable() const;
    std::string getName() const;

    RIPProtocol* getRIP();
};

#endif // ROUTER_H
