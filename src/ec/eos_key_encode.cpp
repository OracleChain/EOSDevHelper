#include "eos_key_encode.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
}

std::vector<unsigned char> eos_key::get_public_key_by_pri(const std::vector<unsigned char>& pri)
{
    std::vector<unsigned char> result;
    uint8_t pub[64];
    unsigned char * priKey = new unsigned char [pri.size() + 1];
    int i;
    for (i = 0; i < (int)pri.size(); ++i) {
        priKey[i] = pri.at(i);
    }
    priKey[i] = '\0';

    uECC_compute_public_key(priKey , pub);
    for (auto c: pub) {
        result.push_back(c);
    }

    delete priKey;

    return result;
}

std::string eos_key::get_eos_public_key_by_wif(const std::string& wif)
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
    unsigned char lpk[100] = { 0 };
    size_t lpklen = 37;
    std::vector<unsigned char> result;
    if(!(b58tobin(lpk, &lpklen, epk.c_str(), epk.size()))){
        return result;
    }

    for (int j = 0; j < 33; ++j) {
        result.push_back(lpk[j]);
    }

    return result;
}

std::vector<unsigned char> eos_key::get_public_key_by_eos_pub(const std::string &pub)
{
    std::vector<unsigned char> base = eos_key::get_public_key_char(pub);
    unsigned char pk[64];
    uECC_decompress(base.data(), pk);

    std::vector<unsigned char> result;
    for (int i = 0; i < 64; ++i) {
        result.push_back(pk[i]);
    }

    return result;
}
