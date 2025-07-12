// src/ApplicationLayer.cpp
#include "../include/ApplicationLayer.h"
#include <iostream>
#include <unordered_map>

ApplicationLayer::ApplicationLayer(TransportLayer* tl) : transport(tl) {}

void ApplicationLayer::sendHTTPRequest(Device* client, Device* server) {
    std::string request = "GET /index.html HTTP/1.1";
    std::cout << "[APP] HTTP Request: " << request << "\n";

    transport->sendData(client, server, request);

    std::string response = "HTTP/1.1 200 OK\n<html><body>Hello from Server!</body></html>";
    server->setData(response);
    std::cout << "[APP] HTTP Response stored on server: " << response << "\n";
}

void ApplicationLayer::sendDNSRequest(Device* client, const std::string& domainName) {
    static std::unordered_map<std::string, std::string> dnsTable = {
        {"yahoo.com", "192.168.1.10"},
        {"google.com", "192.168.1.11"},
        {"example.com", "192.168.1.12"}
    };

    std::cout << "[APP] DNS Query: " << domainName << "\n";
    if (dnsTable.find(domainName) != dnsTable.end()) {
        std::string resolvedIP = dnsTable[domainName];
        std::cout << "[APP] Resolved " << domainName << " to " << resolvedIP << "\n";
        client->setData("Resolved IP: " + resolvedIP);
    } else {
        std::cout << "[APP] Domain not found in DNS table.\n";
        client->setData("Resolution failed.");
    }
}
