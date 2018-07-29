#ifndef EOSWALLET_H
#define EOSWALLET_H

#include <QString>
#include <QMap>
#include <vector>

#include "codebase/ec/eos_key_encode.h"
#include "codebase/ec/sha512.h"

struct wallet_data
{
    std::vector<char> cipher_keys;
};

struct plain_keys
{
    sha512 checksum;
    QMap<QString, QString> keys;
};

class EOSWallet
{    
public:
    EOSWallet();

    void setPassword(const QString& password);
    bool saveFile(const QString& filePath = "");
    bool loadFile(const QString& filePath = "");

    bool importKey(const eos_key& key);
    bool importKey(const QString& wif);

    void lock();
    void unlock(const QString& password);

    bool isNew() const;
    bool isLocked() const;

    QString getPrivateKey(const QString& pubKey) const;

    QMap<QString, QString> listKeys() const;

private:
    void encryptKeys();

private:
    wallet_data             walletData;
    QMap<QString, QString>  keys;
    sha512                  checksum;
};

#endif // EOSWALLET_H
