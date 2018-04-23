#include "eos_key_encode.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aes.hpp"

extern "C"
{

#include "sha2.h"
#include "uECC.h"
#include "libbase58.h"
#include "rmd160.h"

}

/////////////////////////////////////////////////

eos_key::eos_key()
{
    unsigned char str[32] = {0};
    for (int i = 0; i < 32; i++) {
        unsigned char c = (unsigned char)(rand() % 255);
        str[i] = c;
        private_key.push_back(c);
    }

    // Encode private key to wif.
    unsigned char result[37];
    result[0] = 0x80;
    unsigned char digest[32];
    size_t len = 100;
    char base[100] = {0};
    memcpy(result+1, str, 32);
    sha256_Raw(result, 33, digest);
    sha256_Raw(digest, 32, digest);
    memcpy(result+33, digest, 4);
    b58enc(base, &len, result, 37);

    wif_private_key = base;

    // Generate public key.
    uint8_t pub[64];
    uint8_t cpub[33];
    unsigned char *hash = NULL;
    uECC_compute_public_key(str, pub);
    for (auto c : pub) {
        public_key.push_back(c);
    }

    // Encode public key.
    uECC_compress(pub, cpub);
    hash = RMD(cpub, 33);
    memcpy(result, cpub, 33);
    memcpy(result+33, hash, 4);
    b58enc(base, &len, result, 37);

    eos_public_key = "EOS";
    eos_public_key += base;

    free(hash);
}

std::vector<unsigned char> eos_key::get_public_key_by_pri(std::vector<unsigned char> pri)
{
    std::vector<unsigned char> result;
    uint8_t pub[64];
    unsigned char * priKey = new unsigned char [pri.size() + 1];
    int i;
    for (i = 0; i < pri.size(); ++i) {
        priKey[i] = pri.at(i);
    }
    priKey[i] = '\0';

    uECC_compute_public_key(priKey , pub);
    for (auto c: pub) {
        result.push_back(c);
    }

    return result;
}

std::string eos_key::get_eos_public_key_by_wif(std::string wif)
{
    std::string eos_pub;
    unsigned char pri[32];
    const char *baprik = wif.c_str();
    unsigned char result[37];
    unsigned char digest[32];
    char base[100];
    unsigned char *hash;
    size_t len = 100;
    size_t klen = 37;

    uint8_t pub[64];
    uint8_t cpub[33];

    if (b58tobin(result, &klen, baprik, wif.size())) {
        printf("success\n");
    }

    memcpy(pri, result+1, 32);

    uECC_compute_public_key(pri, pub);

    result[0] = 0x80;
    memcpy(result+1, pri, 32);
    sha256_Raw(result, 33, digest);
    sha256_Raw(digest, 32, digest);
    memcpy(result+33, digest, 4);
    b58enc(base, &len, result, 37);

    uECC_compress(pub, cpub);
    hash = RMD(cpub, 33);
    memcpy(result, cpub, 33);
    memcpy(result+33, hash, 4);
    b58enc(base, &len, result, 37);

    eos_pub = "EOS";
    eos_pub += base;

    return eos_pub;
}

std::vector<unsigned char> eos_key::get_private_key_by_wif(const std::string &wif)
{
    std::vector<unsigned char> result;
    if (wif.empty()) {
        return result;
    }

    const char *b58 = wif.c_str();
    unsigned char bin[100] = { 0 };
    size_t binlen = 37;
    b58tobin(bin, &binlen, b58, wif.size());
    if (bin[0] != 0x80) {
        // wrong wif.
        return result;
    }

    unsigned char hexChar[33] = { 0 };
    unsigned char digest[32] = { 0 };
    unsigned char res[32] = { 0 };
    unsigned char last4Bytes[4] = { 0 };
    memcpy(hexChar, bin+1, 32);
    memcpy(last4Bytes, bin+33, 4);
    sha256_Raw(hexChar, 33, digest);
    sha256_Raw(digest, 32, digest);
    memcpy(res, digest, 4);

    if (!strncmp(reinterpret_cast<const char*>(res), reinterpret_cast<const char*>(last4Bytes), 4)) {
        // wif hash validate failed.
        return result;
    }

    for (auto c : hexChar) {
        result.push_back(c);
    }

    return result;
}

std::vector<unsigned char> eos_key::get_public_key_char(const std::string &eoskey){
    std::string epk = eoskey.substr(3,-1);
    unsigned char lpk[100];
    unsigned int lpklen = 37;
    //unsigned char *rhash = nullptr;
    std::vector<unsigned char> result;
    if(!(b58tobin(lpk,&lpklen,epk.c_str(),epk.size()))){
        return result;
    }

    //rhash = RMD(lpk,33);
//    for(int i=0;i<4;i++){
//        if(rhash[i] != lpk[i+33]) return result;
//    }

    for (int j = 0; j < 33; ++j) {
        result.push_back(lpk[j]);
    }

    return result;
}


////////////////////////////////////////////

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

/////////////////////////////////////////////////

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
