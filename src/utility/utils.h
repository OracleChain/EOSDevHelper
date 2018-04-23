#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <stdint.h>

namespace Utils {

    uint32_t convertEndian(uint32_t value);

    std::vector<unsigned char> convertBytesToHexStr(const std::vector<unsigned char>& src);
    std::vector<unsigned char> convertHexStrToBytes(const std::vector<unsigned char>& hex);

}

#endif // UTILS_H
