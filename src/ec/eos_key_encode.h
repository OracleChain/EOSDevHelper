#ifndef EOS_KEY_ENCODE_H
#define EOS_KEY_ENCODE_H

#include <string.h>
#include <string>
#include <vector>

class eos_key
{
public:
    explicit eos_key();

    std::vector<unsigned char> get_private_key() const
    {
        return private_key;
    }

    std::vector<unsigned char> get_public_key() const
    {
        return public_key;
    }

    std::string get_wif_private_key() const
    {
        return wif_private_key;
    }

    std::string get_eos_public_key() const
    {
        return eos_public_key;
    }

    static std::vector<unsigned char> get_public_key_by_pri(std::vector<unsigned char> pri);
    static std::string get_eos_public_key_by_wif(std::string wif);
    static std::vector<unsigned char> get_private_key_by_wif(const std::string& wif);
    static std::vector<unsigned char> get_public_key_char(const std::string &eoskey);

private:
    std::string wif_private_key;
    std::string eos_public_key;

    std::vector<unsigned char> private_key;
    std::vector<unsigned char> public_key;
};

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

#endif // EOS_KEY_ENCODE_H
