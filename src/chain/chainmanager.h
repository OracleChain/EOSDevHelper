#ifndef CHAINMANAGER_H
#define CHAINMANAGER_H

#include "signedtransaction.h"
#include "eosnewaccount.h"
#include <QByteArray>


namespace ChainManager {
    SignedTransaction createTransaction(const std::string& contract, const std::string& actionName,
                                               const std::string& hexData, const std::vector<std::string>& permissions, const QByteArray& info);

    std::vector<std::string> getActivePermission(const std::string& accountName);

    void ValidateSignature(const std::string& raw, const std::string& signature, const std::string& pubKey, std::string& result);
}

#endif // CHAINMANAGER_H
