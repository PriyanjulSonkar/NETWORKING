// src/GoBackN.cpp
#include "../include/GoBackN.h"
#include <iostream>
#include <chrono>
#include <thread>

GoBackN::GoBackN(int winSize)
    : windowSize(winSize), base(0), nextSeqNum(0) {}

void GoBackN::sendData(const std::string& data) {
    std::cout << "[GBN] Starting transmission...\n";

    while (nextSeqNum < data.size()) {
        for (int i = 0; i < windowSize && nextSeqNum + i < data.size(); ++i) {
            char frame = data[nextSeqNum + i];
            std::cout << " → Sent frame [" << nextSeqNum + i << "]: " << frame << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulate delay

        // Simulate all ACKs received for current window
        base = nextSeqNum + windowSize;
        nextSeqNum = base;
        std::cout << " [GBN] All ACKs received for window. Moving base to " << base << "\n";
    }

    std::cout << "[GBN] Transmission complete.\n";
}

void GoBackN::receiveACK(int ackNum) {
    std::cout << "[GBN] Received ACK for frame " << ackNum << "\n";
    if (ackNum >= base) {
        base = ackNum + 1;
        nextSeqNum = base;
    }
}

void GoBackN::receiveNAK(int nakNum) {
    std::cout << "[GBN] NAK received for frame " << nakNum << ". Retransmitting from here.\n";
    nextSeqNum = nakNum;
}
