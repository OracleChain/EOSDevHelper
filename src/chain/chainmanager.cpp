#include "chainmanager.h"
#include "action.h"
#include "signedtransaction.h"
#include "ec/eos_key_encode.h"

namespace ChainManager {

SignedTransaction createTransaction(const std::string &contract, const std::string &actionName, const std::string &hexData, const std::vector<std::string> &permissions, const QByteArray &info)
{
    Action action(contract, actionName);
    action.setAuthorization(permissions);
    action.setData(hexData);

    SignedTransaction txn;
    txn.addAction(action);

    if (!info.isEmpty() && !info.isNull()) {
        QJsonDocument doc = QJsonDocument::fromJson(info);
        QJsonObject obj = doc.object();
        std::string headBlockId = obj.value("head_block_id").toString().toStdString();
        std::string expiration = obj.value("head_block_time").toString().toStdString();

        txn.setReferenceBlock(headBlockId);
        txn.setExpiration(expiration);
    }

    return txn;
}

std::vector<std::string> getActivePermission(const std::string &accountName)
{
    std::vector<std::string> permissions;
    permissions.push_back(accountName + "@active");

    return permissions;
}

void ValidateSignature(const std::string &raw, const std::string &signature, const std::string &pubKey, std::string &result)
{
    result = "unknown error";

    QJsonValue value = QJsonValue(QJsonDocument::fromJson(QByteArray::fromStdString(raw)).object());
    if (value.isNull()) {
        return;
    }

    std::vector<unsigned char> pub = eos_key::get_public_key_by_eos_pub(pubKey);
    std::vector<unsigned char> signatureHex(signature.begin(), signature.end());

    SignedTransaction sigedTxn;
    sigedTxn.fromJson(value);

    if (sigedTxn.signTest(signatureHex, pub, TypeChainId())) {
        result = "Passed, signature and raw action match the public key!";
    } else {
        result = "Failed, signature and raw action do not match the public key!";
    }
}

}

