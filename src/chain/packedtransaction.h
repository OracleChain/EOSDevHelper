#ifndef PACKEDTRANSACTION_H
#define PACKEDTRANSACTION_H

#include "chainbase.h"
#include "signedtransaction.h"

class PackedTransaction : public IFormater
{
private:
    enum class compress_type {
        COMPRESS_NONE,
        COMPRESS_ZLIB
    };

public:
    PackedTransaction(const SignedTransaction& signedTxn, const std::string& compress);

    virtual QJsonValue toJson() const;
    virtual void fromJson(const QJsonValue& value);

private:
    std::vector<unsigned char> packTxn(const SignedTransaction& txn, compress_type cmp);
    std::vector<unsigned char> packCtxFreeData(const std::vector<std::string>& ctxFreeData, compress_type ct);
    std::vector<unsigned char> compress(const std::vector<unsigned char>& src, compress_type cmp);

private:
    std::vector<std::string> signatures;
    std::string compression;
    std::string packed_context_free_data;
    std::string packed_trx;
};

#endif // PACKEDTRANSACTION_H
