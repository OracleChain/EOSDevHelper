#include "eoswallet.h"

#include "packer.h"

#include <QFile>
#include <QMessageBox>

EOSWallet::EOSWallet()
{

}

void EOSWallet::setPassword(const QString &password)
{
    if (!isNew()) {
        if (isLocked()) {
            // The wallet must be unlocked before the password can be set.
            QMessageBox::warning(nullptr, "Error",
                                 "The wallet must be unlocked before the password can be set.");
            return;
        }
    }

    this->checksum = sha512::hash(password.toStdString());
    lock();
}

bool EOSWallet::saveFile(const QString &filePath)
{
    if (filePath.isEmpty()) {
        return false;
    }

    encryptKeys();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    return file.write(walletData.cipher_keys.data(), walletData.cipher_keys.size()) != -1;
}

bool EOSWallet::loadFile(const QString &filePath)
{
    if (filePath.isEmpty()) {
        return false;
    }

    QFile file(filePath);
    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        // Unable to open file.
        return false;
    }

    auto data = file.readAll();
    for (int i = 0; i < data.size(); ++i) {
        walletData.cipher_keys.push_back(data.at(i));
    }

    return true;
}

bool EOSWallet::importKey(const eos_key &key)
{
    if (isLocked()) {
        return false;
    }

    auto eos_pub = QString::fromStdString(key.get_eos_public_key());
    auto itr = this->keys.find(eos_pub);
    if (itr == this->keys.end()) {
        keys.insert(eos_pub, QString::fromStdString(key.get_wif_private_key()));
        return true;
    }

    return false;
}

bool EOSWallet::importKey(const QString &wif)
{
    if (isLocked() || wif.isEmpty()) {
        return false;
    }

    auto eos_pub = QString::fromStdString(eos_key::get_eos_public_key_by_wif(wif.toStdString()));
    if (eos_pub.isEmpty()) {
        return false;
    }

    auto itr = this->keys.find(eos_pub);
    if (itr == this->keys.end()) {
        keys.insert(eos_pub, wif);

        saveFile();
        return true;
    }

    return false;
}

void EOSWallet::lock()
{
    if (isLocked()) {
        return;
    }

    encryptKeys();

    keys.clear();
    checksum = sha512();
}

void EOSWallet::unlock(const QString &password)
{
    if (!password.length()) {
        return;
    }

    auto pw         = sha512::hash(password.toStdString());
    auto decrypted  = aes_decrypt(pw, walletData.cipher_keys);
    plain_keys pk;
    Packer::unpack(decrypted, pk);
    this->keys  = pk.keys;
    checksum    = pk.checksum;
}

bool EOSWallet::isNew() const
{
    return keys.size() == 0;
}

bool EOSWallet::isLocked() const
{
    return checksum == sha512();
}

QString EOSWallet::getPrivateKey(const QString &pubKey) const
{
    if (keys.find(pubKey) == keys.end())
        return QString();

    return keys.value(pubKey);
}

QMap<QString, QString> EOSWallet::listKeys() const
{
    return keys;
}

void EOSWallet::encryptKeys()
{
    if (isLocked()) {
        return;
    }

    plain_keys data;
    data.keys               = this->keys;
    data.checksum           = this->checksum;
    auto plain_text         = Packer::pack(data);
    walletData.cipher_keys  = aes_encrypt(data.checksum, plain_text);
}
