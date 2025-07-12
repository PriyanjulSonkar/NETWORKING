#include "../include/CRC.h"
using namespace std;

string CRC::textToBinary(const string& text) {
    string binaryString = "";
    for (char ch : text) {
        binaryString += bitset<8>(ch).to_string();
    }
    return binaryString;
}

string CRC::binaryToText(const string& binary) {
    string text = "";
    for (size_t i = 0; i < binary.length(); i += 8) {
        string binaryChunk = binary.substr(i, 8);
        char ch = static_cast<char>(bitset<8>(binaryChunk).to_ulong());
        text += ch;
    }
    return text;
}

string CRC::xorOperation(const string& a, const string& b) {
    string result = "";
    for (int i = 1; i < b.length(); i++)
        result += (a[i] == b[i]) ? '0' : '1';
    return result;
}

string CRC::mod2div(const string& dividend, const string& divisor) {
    int pick = divisor.length();
    string tmp = dividend.substr(0, pick);

    while (pick < dividend.length()) {
        if (tmp[0] == '1')
            tmp = xorOperation(tmp, divisor) + dividend[pick];
        else
            tmp = xorOperation(tmp, string(divisor.length(), '0')) + dividend[pick];
        pick++;
    }

    if (tmp[0] == '1')
        tmp = xorOperation(tmp, divisor);
    else
        tmp = xorOperation(tmp, string(divisor.length(), '0'));

    return tmp;
}

string CRC::encodeData(const string& data, const string& generator) {
    string appendedData = data + string(generator.length() - 1, '0');
    string remainder = mod2div(appendedData, generator);
    return data + remainder;
}

bool CRC::checkError(const string& received, const string& generator) {
    return mod2div(received, generator) == string(generator.length() - 1, '0');
}

string CRC::extractOriginalMessage(const string& received, const string& generator) {
    int crcLength = generator.length() - 1;
    return received.substr(0, received.length() - crcLength);
}
