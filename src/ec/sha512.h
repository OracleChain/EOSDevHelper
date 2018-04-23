#ifndef SHA512_H
#define SHA512_H

#include <string>
#include <vector>

class sha512
{
public:
    enum {
        HASH_SIZE = 8
    };
    sha512();

    static sha512 hash(const char *d, uint32_t dlen);
    static sha512 hash(const std::string& str);

    size_t size() const { return HASH_SIZE; }

    uint64_t _hash[HASH_SIZE];
};

bool operator == (const sha512& h1, const sha512& h2);
bool operator != (const sha512& h1, const sha512& h2);

std::vector<char> aes_encrypt(const sha512& key, const std::vector<char>& plain_text);
std::vector<char> aes_decrypt(const sha512& key, const std::vector<char>& cipher_text);

#endif // SHA512_H
