#ifndef PACKER_H
#define PACKER_H

#include <vector>

struct plain_keys;

namespace Packer {
    std::vector<char> pack(const plain_keys& data);
    void unpack(const std::vector<char>& arr, plain_keys& data);
}

#endif // PACKER_H
