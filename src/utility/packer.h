#ifndef PACKER_H
#define PACKER_H

#include <vector>

class plain_keys;

class Packer
{
public:
    Packer();

    static  std::vector<char> pack(const plain_keys& data);
    static void unpack(const std::vector<char>& arr, plain_keys& data);
};

#endif // PACKER_H
