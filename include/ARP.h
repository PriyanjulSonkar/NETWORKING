// include/ARP.h
#ifndef ARP_H
#define ARP_H

#include <string>
#include <unordered_map>
#include <iostream>

class ARP {
private:
    std::unordered_map<std::string, std::string> table; // IP → MAC

public:
    void cacheEntry(const std::string& ip, const std::string& mac);
    std::string resolve(const std::string& ip) const;
    void printTable() const;
};

#endif // ARP_H
