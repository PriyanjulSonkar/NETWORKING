// include/Device.h
#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class Device {
private:
    std::string name;
    std::string ipAddress;
    std::string macAddress;
    int port;
    std::string dataBuffer;
    std::string data;

public:
    Device(std::string deviceName);

    void setIPAddress(const std::string& ip);
    std::string getIPAddress() const;

    void setMACAddress(const std::string& mac);
    std::string getMACAddress() const;

    void setPort(int portNumber);
    int getPort() const;

    void setData(const std::string& data);
    std::string getData() const;

    std::string getName() const;
};

#endif // DEVICE_H
