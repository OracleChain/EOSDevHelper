#include "sha512.h"
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "sha2.h"
#include "aes.h"
}

sha512::sha512()
{
    memset(_hash, 0, sizeof(_hash));
}

sha512 sha512::hash(const char *d, uint32_t dlen)
{
    sha512 h;
    SHA512_CTX ctx;
    sha512_Init(&ctx);
    sha512_Update(&ctx, (const uint8_t*)d, dlen);
    sha512_Final(&ctx, (uint8_t*)&(h._hash[0]));
    return h;
}

sha512 sha512::hash(const std::string &str)
{
    return hash(str.c_str(), str.size());
}

bool operator == (const sha512 &h1, const sha512 &h2)
{
    return memcmp(h1._hash, h2._hash, sha512::HASH_SIZE) == 0;
}

bool operator != (const sha512 &h1, const sha512 &h2)
{
    return memcmp(h1._hash, h2._hash, sha512::HASH_SIZE) != 0;
}

std::vector<char> aes_encrypt(const sha512& key, const std::vector<char>& plain_text)
{
    int size = plain_text.size() + 16;
    unsigned char *temp = new unsigned char[size];
    memset(temp, 0, size);
    memcpy(temp, plain_text.data(), plain_text.size());

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, (unsigned char*)&key, ((unsigned char *)&key)+32);
    AES_CBC_encrypt_buffer(&ctx, temp, (int)plain_text.size());

    std::vector<char> encrypted;
    for (int i = 0; i < size; ++i) {
        encrypted.push_back((char)temp[i]);
    }

    delete temp;

    return encrypted;
}

std::vector<char> aes_decrypt(const sha512& key, const std::vector<char>& cipher_text)
{
    int size = cipher_text.size();
    unsigned char *temp = new unsigned char[size];
    memcpy(temp, cipher_text.data(), size);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, (unsigned char*)&key, ((unsigned char *)&key)+32);
    AES_CBC_decrypt_buffer(&ctx, temp, size);

    std::vector<char> decrypted;
    for (int i = 0; i < size; ++i) {
        decrypted.push_back((char)temp[i]);
    }

    // delete temp; why crash here for MingW?

    return decrypted;
}
