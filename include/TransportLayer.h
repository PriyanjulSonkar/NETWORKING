// include/TransportLayer.h
#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H

#include <string>
#include <unordered_map>
#include "Device.h"

class GoBackN; // forward declaration (or SelectiveRepeat)

class TransportLayer {
private:
    std::unordered_map<std::string, int> portTable; // DeviceName → Port
    GoBackN* goBackN;

public:
    TransportLayer();

    void openPort(Device* device, int port);
    int getPort(Device* device) const;

    void sendData(Device* sender, Device* receiver, const std::string& data);
    void receiveACK(int ackNum);
    void receiveNAK(int nakNum);
};

#endif // TRANSPORTLAYER_H
