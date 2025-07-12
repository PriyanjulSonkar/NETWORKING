#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include <string>
#include <vector>
#include <utility>

struct RouteEntry {
    std::string network;
    std::string subnetMask;
    std::string nextHop;
    int metric;
    std::string interface;
};

class RoutingTable {
public:
    std::vector<RouteEntry> routes;

    void addRoute(const std::string& network, const std::string& subnetMask,
                  const std::string& nextHop, int metric, const std::string& interface);

    std::pair<std::string, std::string> findBestRoute(const std::string& targetIP) const;
};

bool ipInNetwork(const std::string& ip, const std::string& network, const std::string& subnetMask);

#endif
