#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <bitset>
#include <sstream>

using namespace std;

// Global adjacency list for the entire network
unordered_map<string, unordered_set<string>> networkGraph;
class Hub;  // Forward declaration of Hub
unordered_map<string, Hub*> hubs;  // Now compiler knows about Hub*
class EndDevice;
unordered_map<string, EndDevice*> devices;
class Switch;
unordered_map<string, Switch*> Switches;
string textToBinary(string text);

void bfsMessageTraversal(string sender, string receiver, string data, unordered_set<string>& visited, unordered_map<string, EndDevice*>& devices, unordered_map<string, Hub*>& hubs);


// Base class for devices
class Device {
protected:
    string name;
public:
    Device(string n) : name(n) {}
    string getName() { return name; }
    // virtual void receive(string data, string sender, string receiver) = 0;
};

// End Device class
class EndDevice : public Device {
public:
    EndDevice(string n) : Device(n) {}

    bool receive(string data, string sender, string receiver) {
        if (name == receiver) {
            cout << name << " received message: " << data << " from " << sender << endl;
            if(data != "ACK"){
                cout<<endl;
                cout << name << " sending ACK to " << sender << endl;
            }
            return true;
        } else {
            cout << name << " Simplpy discard it (Not intended receiver)\n";
            return false;
        }
    }

    void sendToNetwork(string data, string receiver) {
        // if(data == "ACK"){
        //     cout << "\n" << name << " sending "<<data<<" to " << receiver << endl;
        // }
        // else{
        //     cout << "\n" << name << " sending message to " << receiver << endl;
        // }
        if(data != "ACK"){
            cout << "\n" << name << " sending message to " << receiver << endl;
        }
        string binaryData = textToBinary(data);
        cout << "Text to Binary: " << binaryData << endl;

        string deviceName;
        for(auto& devi:networkGraph[name]){
            cout<<devi<<endl;  
            deviceName = devi;      
        }
        if (hubs.find(deviceName) != hubs.end()) {
            cout << deviceName << " is a Hub" << endl;
        } 
        else if (Switches.find(deviceName) != Switches.end()) {
            cout << deviceName << " is a Switch" << endl;
            cout<<Switches[deviceName];
            // Switches[deviceName]->forwardData(name, receiver, binaryData);
        } 
        else if (devices.find(deviceName) != devices.end()) {
            cout << deviceName << " is an End Device" << endl;
        } 
        else {
            cout << deviceName << " not found in any category!" << endl;
        }
        
        
        // unordered_set<string> visited;
        // bfsMessageTraversal(name, receiver, data, visited,devices,hubs);
        
    }

};

    // BFS to traverse the network and deliver the message
void bfsMessageTraversal(string sender, string receiver, string data, unordered_set<string>& visited,unordered_map<string, EndDevice*>& devices,unordered_map<string, Hub*>& hubs) {
        queue<pair<string, string>> q; // {current node, parent sender}
        q.push({sender, sender});
        visited.insert(sender);
    
        while (!q.empty()) {
            auto [current, parentSender] = q.front();
            q.pop();
    
            for (const string& neighbor : networkGraph[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    cout << "Broadcasting to " << neighbor << " from " << current << endl;
                    
                    // ✅ Neighbor ka actual EndDevice object dhundho aur receive() call karo
                    if (devices.find(neighbor) != devices.end()) {
                        devices[neighbor]->receive(data, sender, receiver);
                    }
    
                    // if (neighbor == receiver) {
                    //     if(data == "ACK"){
                    //         cout << receiver << " received " << data << ": from " << sender << endl;
                    //     }
                    //     else{
                    //         cout << receiver << " received message: " << data << " from " << sender << endl;
                    //     }
                        
                    //     // cout << receiver << " sending ACK to " << sender << endl;
                        
                    //     // return; // Stop after reaching the receiver
                    // } else if(data == "ACK"){
                    //     cout << current << " sending NACK to "<<neighbor<<"\n";
                    // }                
    
                    q.push({neighbor, current});
                }
            }
        }
    }

// Hub class
class Hub : public Device {
public:
    Hub(string n) : Device(n) {}

    void receive(string data, string sender, string receiver) {
        cout << "Hub " << name << " received data from " << sender << " for " << receiver << endl;
    }
};

// Switch Class
class Switch : public Device {
    // Switch(string n) : Device(n) {}
    private:
        unordered_map<string, string> macTable; // MAC Address Table
    
    public:
    Switch(string n) : Device(n) {}
        void learnMAC(string device, string mac) {
            if (macTable.find(device) == macTable.end()) {
                macTable[device] = mac;
                cout << "Switch learned MAC: " << mac << " for " << device << endl;
            }
        }
        void forwardData(string sender, string receiver, string data) {
            cout << "\nSwitch received data from " << sender << " for " << receiver << endl;
            
            // MAC Address Learning
            learnMAC(sender, "MAC_" + sender);
    
            // Agar destination MAC address table me hai
            if (macTable.find(receiver) != macTable.end()) {
                cout << "Switch forwarding data directly to " << receiver << endl;
            }
            // Agar destination unknown hai to flooding karna hoga
            else {
                cout << "Switch flooding data to all connected devices except " << sender << endl;
                for (const string& device : networkGraph[name]) { // Switch ke connected devices
                    if (device != sender) {
                        // cout << "Sending to " << device << endl;
    
                        // // Simulating device response (Assume receiver replies with MAC address)
                        // if (device == receiver) {
                        //     learnMAC(receiver, "MAC_" + receiver);
                        //     cout << receiver << " replied with its MAC. Now switch has learned it!\n";
                        // }
                        bool res = false;
                        if (devices.find(device) != devices.end()) {
                           res = devices[device]->receive(data, sender, receiver);
                        }
                        if(res){
                                learnMAC(receiver, "MAC_" + receiver);
                            cout << receiver << " replied with its MAC. Now switch has learned it!\n";
                        }
                    }
                }
            }
        }
    };

    class Bridge :public Device{
        private:
            unordered_map<string, string> macTable; // MAC Address Table
            unordered_map<string, int> portTable; // Device to Segment Mapping
        
        public:
            Bridge(string n) : Device(n) {}
        
            void connectDevice(string device, int segment) {
                networkGraph[name].insert(device);
                networkGraph[device].insert(name);
                portTable[device] = segment;
            }
        
            void learnMAC(string sender) {
                if (macTable.find(sender) == macTable.end()) {
                    macTable[sender] = "MAC_" + sender;
                    cout << "Bridge learned MAC: " << macTable[sender] << " for " << sender << endl;
                }
            }
        
            void forwardData(string sender, string receiver, string data) {
                cout << "\nBridge received data from " << sender << " for " << receiver << endl;
        
                // Learning sender's MAC address
                learnMAC(sender);
        
                // Check if both sender and receiver are in the same segment
                if (portTable[sender] == portTable[receiver]) {
                    cout << "Bridge blocking data (same segment: " << portTable[sender] << ")" << endl;
                    return;
                }
        
                // If receiver MAC is known, forward data directly
                if (macTable.find(receiver) != macTable.end()) {
                    cout << "Bridge forwarding data to other segment " << portTable[receiver] << endl;
                }
                // Otherwise, flood to other segment
                else {
                    cout << "Bridge flooding data to other segment (learning mode)" << endl;
                    for (auto& entry : portTable) {
                        if (entry.first != sender && entry.second != portTable[sender]) {
                            cout << "Sending to " << entry.first << endl;
        
                            // Simulating receiver response
                            if (entry.first == receiver) {
                                learnMAC(receiver);
                                cout << receiver << " replied. Now bridge has learned its MAC!\n";
                            }
                        }
                    }
                }
            }
        };

        class CSMA_CD {
            private:
                bool channelBusy; // Track whether the channel is busy or free
                int activeTransmitters; // Number of devices currently transmitting
            
            public:
                CSMA_CD() {
                    channelBusy = false;
                    activeTransmitters = 0;
                    srand(time(0)); // Random seed for collision simulation
                }
            
                void sendData(string sender, string receiver, string data) {
                    cout<<endl;
                    cout << sender << " is trying to send data to " << receiver << endl;
            
                    // Step 1: Carrier Sense (Check if channel is free)
                    if (channelBusy) {
                        cout << "Channel is busy. " << sender << " is waiting..." << endl;
                        waitForChannel(sender);
                    }
            
                    // Step 2: Start Transmission
                    cout << sender << " starts transmitting data..." << endl;
                    channelBusy = true;
                    activeTransmitters++;
            
                    // Step 3: Simulate Collision Only If Multiple Devices Are Transmitting
                    if (activeTransmitters > 1) {
                        cout << "Collision detected! " << sender << " stops transmission." << endl;
                        handleCollision(sender);
                    } else {
                        // No collision, successful transmission
                        this_thread::sleep_for(chrono::milliseconds(500)); // Simulating transmission time
                        cout << "Data successfully sent from " << sender << " to " << receiver << ": " << data << endl;
                        activeTransmitters--;
                        if (activeTransmitters == 0) {
                            channelBusy = false;
                        }
                    }
                }
            
                void waitForChannel(string sender) {
                    while (channelBusy) {
                        cout << sender << " is waiting for the channel to be free..." << endl;
                        this_thread::sleep_for(chrono::milliseconds(500)); // Simulate wait time
                    }
                }
            
                void handleCollision(string sender) {
                    int backoffTime = (rand() % 5 + 1) * 500; // Random backoff time (500ms - 2500ms)
                    cout << sender << " is waiting for " << backoffTime / 1000.0 << " seconds before retransmitting..." << endl;
                    this_thread::sleep_for(chrono::milliseconds(backoffTime));
                    activeTransmitters--;
                    if (activeTransmitters == 0) {
                        channelBusy = false;
                    }
                    cout << sender << " is retrying to send data..." << endl;
                    sendData(sender, "Receiver", "Hello Again!"); // Retry sending after backoff
                }
            };


            class SelectiveRepeat {
                private:
                    int windowSize;
                    int totalFrames;
                    vector<bool> receivedFrames;
                    vector<bool> sentFrames;
                    unordered_map<int, chrono::time_point<chrono::steady_clock>> frameTimers;
                
                public:
                    SelectiveRepeat(int ws, int tf) {
                        windowSize = ws;
                        totalFrames = tf;
                        receivedFrames.resize(totalFrames, false); // Initially all frames are unreceived
                        sentFrames.resize(totalFrames, false);    // Initially all frames are unsent
                        srand(time(0)); // Random seed for error simulation
                    }
                
                    void sendFrames() {
                        int base = 0; // Window ka starting index
                
                        while (base < totalFrames) {
                            cout << "\nSending frames: ";
                            for (int i = base; i < min(base + windowSize, totalFrames); i++) {
                                if (!sentFrames[i]) {
                                    cout << i << " ";
                                    frameTimers[i] = chrono::steady_clock::now(); // Start timer
                                }
                            }
                            cout << endl;
                
                            // Simulate receiver behavior
                            receiveFrames(base);
                
                            // Timeout check
                            checkTimeouts();
                
                            // Slide window forward
                            while (base < totalFrames && receivedFrames[base]) {
                                base++;
                            }
                
                            this_thread::sleep_for(chrono::milliseconds(1000)); // Delay for better visualization
                        }
                    }
                
                    void receiveFrames(int start) {
                        for (int i = start; i < min(start + windowSize, totalFrames); i++) {
                            if (!sentFrames[i]) {
                                if (rand() % 5 == 0) { // 20% chance frame lost
                                    cout << "Frame " << i << " lost!" << endl;
                                } else {
                                    cout << "Frame " << i << " received successfully!" << endl;
                                    receivedFrames[i] = true;
                                    sentFrames[i] = true;
                                    frameTimers.erase(i); // Frame received, remove timer
                                }
                            }
                        }
                    }
                
                    void checkTimeouts() {
                        auto now = chrono::steady_clock::now();
                        for (auto it = frameTimers.begin(); it != frameTimers.end();) {
                            int frame = it->first;
                            auto startTime = it->second;
                
                            if (chrono::duration_cast<chrono::milliseconds>(now - startTime).count() > 50) { // 2 sec timeout
                                cout << "Timeout! Retransmitting Frame " << frame << endl;
                                this_thread::sleep_for(chrono::milliseconds(500));
                                frameTimers[frame] = chrono::steady_clock::now(); // Restart Timer
                                receiveFrames(frame);
                                it = frameTimers.erase(it); // Remove timer after retransmission
                            } else {
                                ++it;
                            }
                        }
                    }
                };
            
        

// Function to add a connection in the global network graph
void connect(string device1, string device2) {
    networkGraph[device1].insert(device2);
    networkGraph[device2].insert(device1);
}

// Function to display the network graph
void displayNetwork() {
    cout << "\nNetwork Adjacency List:\n";
    for (auto& pair : networkGraph) {
        cout << pair.first << " -> ";
        for (const string& neighbor : pair.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// Function to perform XOR operation
string xorOperation(string a, string b) {
    string result = "";
    for (int i = 1; i < b.length(); i++)  // First bit ignore (division rule)
        result += (a[i] == b[i]) ? '0' : '1';
    return result;
}

void print(string& tmp){
    
    for(int i = 0;i<tmp.length();i++){
        cout<<tmp[i];
    }
    cout<<endl;
}
// Function to perform Modulo-2 Division
string mod2div(string dividend, string divisor) {
    int pick = divisor.length();  // Length of divisor
    string tmp = dividend.substr(0, pick);  // Initial chunk of dividend

    cout << "Initial tmp: " << tmp << endl;

    while (pick < dividend.length()) {
        // Perform XOR if first bit is 1, otherwise XOR with 0s
        if (tmp[0] == '1')
            tmp = xorOperation(tmp, divisor) + dividend[pick];
        else
            tmp = xorOperation(tmp, string(divisor.length(), '0')) + dividend[pick];

        cout << "Updated tmp: " << tmp << endl;

        pick++; // Move window
    }

    // Final XOR step
    if (tmp[0] == '1')
        tmp = xorOperation(tmp, divisor);
    else
        tmp = xorOperation(tmp, string(divisor.length(), '0'));

    cout << "Final Remainder: " << tmp << endl;
    return tmp;
}


// Function to encode data using CRC
string encodeData(string data, string generator) {
    string appendedData = data + string(generator.length() - 1, '0');
    string remainder = mod2div(appendedData, generator);
    return data + remainder;
}

// Function to check if error is present in received data
bool checkError(string received, string generator) {
    return mod2div(received, generator) == string(generator.length() - 1, '0');
}

// Function to calculate broadcast and collision domains
void calculateDomains() {
    int broadcastDomains = 1;  // Only broken by routers
    int collisionDomains = 0;

    for (auto& pair : networkGraph) {
        if (pair.first.find("Hub") != string::npos) {
            cout << "Hub " << pair.first << " forms **1 Collision Domain**\n";
            collisionDomains += 1;
        } else if (pair.first.find("Switch") != string::npos) {
            cout << "Switch " << pair.first << " creates **separate collision domains** for each port\n";
            collisionDomains += pair.second.size();
        }
    }

    cout << "\nTotal Broadcast Domains: " << broadcastDomains << endl;
    cout << "Total Collision Domains: " << collisionDomains << endl;
}

// **Function to Convert Text to Binary (ASCII Based)**
string textToBinary(string text) {
    string binaryString = "";
    for (char ch : text) {
        binaryString += bitset<8>(ch).to_string() + " "; // 8-bit binary representation
    }
    return binaryString;
}

// **Function to Convert Binary to Text (ASCII Based)**
string binaryToText(string binary) {
    stringstream ss(binary);
    string binaryChunk, text = "";

    while (ss >> binaryChunk) {
        char ch = static_cast<char>(bitset<8>(binaryChunk).to_ulong());
        text += ch;
    }

    return text;
}

int main() {
    // unordered_map<string, EndDevice*> devices;
    // unordered_map<string, Hub*> hubs;

    // // Creating Hubs
    // hubs["Hub1"] = new Hub("Hub1");
    // hubs["Hub2"] = new Hub("Hub2");

    // Creating End Devices
    // devices["PC1"] = new EndDevice("PC1");
    // devices["PC2"] = new EndDevice("PC2");
    // devices["PC3"] = new EndDevice("PC3");
    // devices["PC4"] = new EndDevice("PC4");

    // // Connecting Devices to Hubs (Using Global Graph)
    // connect("Hub1", "PC1");
    // connect("Hub1", "PC2");
    // connect("Hub2", "PC3");
    // connect("Hub1", "Hub2"); // Connecting hubs

    // // Displaying Network
    // displayNetwork();

    // // Sending Message from PC1 to PC3
    // string sender = "PC1", receiver = "PC3", message = "Hello PC3!";
    // devices[sender]->sendToNetwork(message, receiver,devices,hubs);
    // devices[receiver]->sendToNetwork("ACK", sender,devices,hubs);
    // // Cleanup
    // for (auto& pair : devices) {
    //     delete pair.second;
    // }
    // for (auto& pair : hubs) {
    //     delete pair.second;
    // }

    // Switch sw("Switch1");
    Switches["Switch1"] = new Switch("Switch1");

    // // Connecting Devices
    // connect("Switch1", "PC1");
    // connect("Switch1", "PC2");
    // connect("Switch1", "PC3");
    // connect("Switch1", "PC4");

    // // Displaying Network
    // displayNetwork();

    // // Sending Messages
    // sw.forwardData("PC1", "PC3", "Hello PC3!");
    // cout<<endl;
    // // sw.forwardData("PC3", "PC1", "Hello PC1!");
    // sw.forwardData("PC4", "PC3", "Hi PC3!");


    // Bridge bridge("brick");

    // // Connecting Devices to Bridge (2 Segments)
    // bridge.connectDevice("PC1", 1);
    // bridge.connectDevice("PC2", 1);
    // bridge.connectDevice("PC3", 2);
    // bridge.connectDevice("PC4", 2);

    // // Displaying Network
    // displayNetwork();

    // // Sending Messages
    // bridge.forwardData("PC1", "PC2", "Hello PC2!");  // Blocked (same segment)
    // cout<<endl;
    // bridge.forwardData("PC1", "PC3", "Hello PC3!");  // Flooding (PC3 unknown)
    // cout<<endl;
    // bridge.forwardData("PC3", "PC1", "Hello PC1!");  // Now direct forward
    // cout<<endl;
    // bridge.forwardData("PC4", "PC1", "Hi PC1!");     // Flooding (PC1 unknown)
    // cout<<endl;
    // bridge.forwardData("PC1", "PC4", "Hi PC4!");     // Now direct forward
    // cout<<endl;


    // string data = "1011001"; // Example Data Bits
    // string generator = "1001"; // CRC Generator Polynomial

    // // Encoding data
    // string encodedData = encodeData(data, generator);
    // cout << "Encoded Data (Sent): " << encodedData << endl;

    // // Simulating transmission with errors
    // string receivedData = encodedData;
    // receivedData[2] = (receivedData[2] == '0') ? '1' : '0'; // Injecting error in transmission

    // // Checking for errors
    // if (checkError(receivedData, generator))
    //     cout << "No Error Detected in Received Data!" << endl;
    // else
    //     cout << "Error Detected in Received Data!" << endl;


    // CSMA_CD network;

    // // Simulating multiple devices trying to send data
    // thread t1([&]() { network.sendData("PC1", "PC2", "Hello, PC2!"); });
    // thread t2([&]() { network.sendData("PC3", "PC4", "Hey, PC4!"); });

    // t1.join();
    // t2.join();

    // int windowSize = 4; // Maximum frames sent at a time
    // int totalFrames = 10; // Total frames to send

    // SelectiveRepeat sr(windowSize, totalFrames);
    // sr.sendFrames();

    for (int i = 1; i <= 5; i++) {
        string devName = "PC" + to_string(i);
        devices[devName] = new EndDevice(devName);
        connect("Switch1", devName);
    }

    displayNetwork();

    string sender = "PC1";
    string message = "hello dear";
    // cout << "Enter message: ";
    // cin.ignore();
    // getline(cin, message);
    string receiver = "PC4";
    cout<<devices[sender]<<endl;
    devices[sender]->sendToNetwork(message,receiver);

    


    return 0;
}
