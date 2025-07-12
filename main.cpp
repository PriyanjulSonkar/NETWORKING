#include <iostream>
#include "include/Device.h"
#include "include/Router.h"
#include "include/Switch.h"
#include "include/DHCP.h"
#include "include/ARP.h"
#include "include/MACAddress.h"
#include "include/TransportLayer.h"
#include "include/ApplicationLayer.h"
#include "include/CRC.h"
#include "include/NetworkLayer.h"

int main() {
    std::cout << "\n======== Network Simulation: =========\n";

    // Create Devices
    Device PCA("PCA"), PCB("PCB");
    Router router("MainRouter", nullptr); // networkLayer unused for now

    //  Assign MACs to all
    PCA.setMACAddress(MACAddress::generate());
    PCB.setMACAddress(MACAddress::generate());
    router.setMACAddress(MACAddress::generate());

    // Create Switches
    Switch switchA, switchB;

    // Connect devices to switches
    switchA.connect(&PCA);
    switchA.connect(&router);
    switchB.connect(&PCB);
    switchB.connect(&router);

    std::cout << "[DEBUG] Physical connections established\n";

    // Assign IPs via DHCP
    DHCP dhcp;
    dhcp.requestIP(&PCA);
    dhcp.requestIP(&PCB);
    dhcp.requestIP(&router, "eth0");
    dhcp.requestIP(&router, "eth1");

    std::cout << "[DEBUG] DHCP IP assignment complete\n";

    // ARP setup
    ARP arp;
    arp.cacheEntry(PCA.getIPAddress(), PCA.getMACAddress());
    arp.cacheEntry(PCB.getIPAddress(), PCB.getMACAddress());
    router.receiveARPReply(PCA.getIPAddress(), PCA.getMACAddress());
    router.receiveARPReply(PCB.getIPAddress(), PCB.getMACAddress());

    std::cout << "[DEBUG] ARP table updated\n";

    //  RIP Routing
    router.getRIP()->addNeighbor(PCA.getIPAddress(), "eth0");
    router.getRIP()->addNeighbor(PCB.getIPAddress(), "eth1");
    router.getRIP()->advertiseRoutes();

    std::cout << "[DEBUG] RIP neighbors added and routes advertised\n";

    // Transport Layer setup
    TransportLayer transport;
    transport.openPort(&PCA, 1234); // ephemeral
    transport.openPort(&PCB, 80);   // HTTP port

    //  Application Layer communication
    ApplicationLayer app(&transport);
    app.sendHTTPRequest(&PCA, &PCB);
    app.sendDNSRequest(&PCA, "example.com");

    std::cout << "[DEBUG] Application communication simulated\n";

    //  CRC Demonstration
    std::string message = "HelloPCB";
    std::string binary = CRC::textToBinary(message);
    std::string encoded = CRC::encodeData(binary, "1001");
    std::cout << "\n[CRC Test] PCA sending encoded message to PCB...\n";
    PCB.setData(encoded);  // Receiver side will verify CRC and print

    std::cout << "\n Simulation Completed Successfully!\n";

    return 0;
}
