#ifndef CRC_H
#define CRC_H

#include <string>
#include <bitset>

class CRC {
public:
    static std::string textToBinary(const std::string& text);
    static std::string binaryToText(const std::string& binary);
    static std::string xorOperation(const std::string& a, const std::string& b);
    static std::string mod2div(const std::string& dividend, const std::string& divisor);
    static std::string encodeData(const std::string& data, const std::string& generator);
    static bool checkError(const std::string& received, const std::string& generator);
    static std::string extractOriginalMessage(const std::string& received, const std::string& generator);
};

#endif // CRC_H
