#ifndef SIGNEDTRANSACTION_H
#define SIGNEDTRANSACTION_H

#include "transaction.h"
#include "ec/typechainid.h"


class SignedTransaction : public Transaction
{
public:
    SignedTransaction();

    virtual QJsonValue toJson();
    virtual void fromJson(const QJsonValue& value);

    std::vector<std::string> getSignatures() const;
    std::vector<std::string> getCtxFreeData() const;
    void setSignatures(const std::vector<std::string>& signatures);
    void sign(const std::vector<unsigned char>& pri_key, const TypeChainId& cid);
    bool signTest(const std::vector<unsigned char>& signature, const std::vector<unsigned char>& pubKey, const TypeChainId &cid);

private:
    std::vector<unsigned char> getDigestForSignature(const TypeChainId& cid);

private:
    std::vector<std::string> signatures;
    std::vector<std::string> context_free_data;
};

#endif // SIGNEDTRANSACTION_H
