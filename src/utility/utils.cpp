#include "utils.h"
#include <stdio.h>

namespace Utils {

uint32_t convertEndian(uint32_t value)
{
    return ((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) << 8) |
            ((value & 0x00FF0000) >> 8) |
            ((value & 0xFF000000) >> 24) ;
}

std::vector<unsigned char> convertBytesToHexStr(const std::vector<unsigned char> &src)
{
    std::vector<unsigned char> dst;
    for (int i = 0; i < (int)src.size(); ++i) {
        unsigned char hex[2];
        unsigned char c = src.at(i);
        sprintf((char*)&hex[0], "%02x", c);
        dst.push_back(hex[0]);
        dst.push_back(hex[1]);
    }

    return dst;
}

std::vector<unsigned char> convertHexStrToBytes(const std::vector<unsigned char> &hex)
{
    std::vector<unsigned char> dst;
    for (int i = 0; i < (int)hex.size(); i += 2) {
        unsigned char c = (hex.at(i) % 32 + 9) % 25 * 16 + (hex.at(i+1) % 32 + 9) % 25;
        dst.push_back(c);
    }

    return dst;
}

}

