// include/ApplicationLayer.h
#ifndef APPLICATIONLAYER_H
#define APPLICATIONLAYER_H

#include <string>
#include "Device.h"
#include "TransportLayer.h"

class ApplicationLayer {
private:
    TransportLayer* transport;

public:
    ApplicationLayer(TransportLayer* tl);

    void sendHTTPRequest(Device* client, Device* server);
    void sendDNSRequest(Device* client, const std::string& domainName);
};

#endif // APPLICATIONLAYER_H
