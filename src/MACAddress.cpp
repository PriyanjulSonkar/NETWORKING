// src/MACAddress.cpp
#include "../include/MACAddress.h"
#include <sstream>
#include <iomanip>
#include <random>

std::string MACAddress::generate() {
    std::random_device rd; // Hardware-based randomness
    std::mt19937 gen(rd()); //Mersenne Twister (fast pseudo-random number generator)
    std::uniform_int_distribution<> dis(0, 255); //uniform distribution banaya gaya jo 0 se 255 ke beech

    std::ostringstream mac;
    for (int i = 0; i < 6; ++i) {
        if (i != 0) mac << ":";
        mac << std::setw(2) << std::setfill('0') << std::hex << dis(gen);
    }
    return mac.str();
}
