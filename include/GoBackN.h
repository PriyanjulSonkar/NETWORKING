// include/GoBackN.h
#ifndef GOBACKN_H
#define GOBACKN_H

#include <string>

class GoBackN {
private:
    int windowSize;
    int base;
    int nextSeqNum;

public:
    GoBackN(int winSize);

    void sendData(const std::string& data);
    void receiveACK(int ackNum);
    void receiveNAK(int nakNum);
};

#endif // GOBACKN_H
