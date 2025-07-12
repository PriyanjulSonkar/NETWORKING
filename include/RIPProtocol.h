// include/RIPProtocol.h
#ifndef RIPPROTOCOL_H
#define RIPPROTOCOL_H

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

class Router; // forward declaration

class RIPProtocol {
private:
    Router* router;
    std::unordered_map<std::string, std::string> neighbors; // neighborIP → interface

public:
    RIPProtocol(Router* routerPtr);

    void addNeighbor(const std::string& neighborIP, const std::string& interface);
    void receiveRouteUpdate(const std::string& fromNeighborIP,
                            const std::vector<std::tuple<std::string, std::string, int>>& routes);

    void advertiseRoutes() const;
    void printNeighbors() const;
    const std::unordered_map<std::string, std::string>& getNeighbors() const;
};

#endif // RIPPROTOCOL_H
