#include "signedtransaction.h"
#include "eosbytewriter.h"
#include "Crypto/libbase58.h"
#include "utility/utils.h"

extern "C"
{
#include "Crypto/sha2.h"
#include "Crypto/uECC.h"
#include "rmd160.h"
}

SignedTransaction::SignedTransaction()
{

}

QJsonValue SignedTransaction::toJson()
{
    QJsonObject obj = Transaction::toJson().toObject();
    QJsonArray signaturesArr, ctxFreeDataArr;
    for (const auto& s : signatures) {
        signaturesArr.append(QJsonValue(s.c_str()));
    }

    for (const auto& d : context_free_data) {
        ctxFreeDataArr.append(QJsonValue(d.c_str()));
    }

    obj.insert("signatures", signaturesArr);
    obj.insert("context_free_data", ctxFreeDataArr);

    return QJsonValue(obj);
}

void SignedTransaction::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    if (obj.isEmpty()) {
        return;
    }

    QJsonArray signaturesArr = obj.value("signatures").toArray();
    if (!signaturesArr.isEmpty()) {
        for (int i = 0; i < signaturesArr.size(); ++i) {
            signatures.push_back(signaturesArr.at(i).toString().toStdString());
        }
    }

    QJsonArray ctxFreeDataArr = obj.value("context_free_data").toArray();
    if (!ctxFreeDataArr.isEmpty()) {
        for (int i = 0; i < ctxFreeDataArr.size(); ++i) {
            context_free_data.push_back(ctxFreeDataArr.at(i).toString().toStdString());
        }
    }

    Transaction::fromJson(value);
}

std::vector<std::string> SignedTransaction::getSignatures() const
{
    return signatures;
}

std::vector<std::string> SignedTransaction::getCtxFreeData() const
{
    return context_free_data;
}

void SignedTransaction::setSignatures(const std::vector<std::string> &signatures)
{
    this->signatures = signatures;
}

void SignedTransaction::sign(const std::vector<unsigned char> &pri_key, const TypeChainId &cid)
{
    std::vector<unsigned char> packedBytes = getDigestForSignature(cid);

    uint8_t packedSha256[SHA256_DIGEST_LENGTH];
    sha256_Raw(packedBytes.data(), packedBytes.size(), packedSha256);

    uint8_t signature[uECC_BYTES * 2];
    int recId = uECC_sign_forbc(pri_key.data(), packedSha256, signature);
    if (recId == -1) {
        // could not find recid, data probably already signed by the key before?
        return;
    } else {
        unsigned char bin[65+4] = { 0 };
        unsigned char *rmdhash = NULL;
        int binlen = 65+4;
        int headerBytes = recId + 27 + 4;
        bin[0] = (unsigned char)headerBytes;
        memcpy(bin + 1, signature, uECC_BYTES * 2);

        rmdhash = RMD(bin,65);
        memcpy(bin + 1 +  uECC_BYTES * 2, rmdhash, 4);

        char sigbin[100] = { 0 };
        size_t sigbinlen = 100;
        b58enc(sigbin, &sigbinlen, bin, binlen);
        std::string sig = "SIG_K1_";
        sig += sigbin;

        signatures.push_back(sig);
    }
}

bool SignedTransaction::signTest(const std::vector<unsigned char> &signatureHex, const std::vector<unsigned char> &pubKey, const TypeChainId &cid)
{
    std::vector<unsigned char> packedBytes = getDigestForSignature(cid);
    uint8_t packedSha256[SHA256_DIGEST_LENGTH];
    sha256_Raw(packedBytes.data(), packedBytes.size(), packedSha256);

    std::vector<unsigned char> signature = Utils::convertHexStrToBytes(signatureHex);
    signature.erase(signature.begin());   //remove headerBytes

    return checkSignature(pubKey.data(), packedSha256, signature.data()) != -1;
}

std::vector<unsigned char> SignedTransaction::getDigestForSignature(const TypeChainId &cid)
{
    EOSByteWriter writer(255);
    writer.putBytes(cid.chainId(), cid.size());

    serialize(&writer);

    return writer.toBytes();
}
