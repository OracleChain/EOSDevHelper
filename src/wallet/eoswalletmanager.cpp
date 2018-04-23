#include "eoswalletmanager.h"
#include "ec/eos_key_encode.h"
#include "eoswallet.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

EOSWalletManager::EOSWalletManager()
{
    defaultExists = false;
    dir = QDir::currentPath() + "/data";
    if (!QFileInfo(dir).exists()) {
        QDir curDir;
        bool ret = curDir.mkpath(dir);
        Q_ASSERT(ret);
    }

    loadPasswords();
}

EOSWalletManager &EOSWalletManager::instance()
{
    static EOSWalletManager manager;
    return manager;
}

QString EOSWalletManager::create(const QString &name)
{
    QString password = genPassword();
    if (dir.at(dir.length() - 1) != '/' ||
            dir.at(dir.length() - 1) != '\\')
        dir += '/';
    QFileInfo fileInfo(dir + name + EOS_WALLET_FILE_EXT);
    if (fileInfo.exists()) {
        QMessageBox::critical(NULL, "Error", "Wallet " + name + " already exists!");
        return QString();
    }

    EOSWallet wallet;
    wallet.setPassword(password);
    wallet.setWalletFilePath(fileInfo.absoluteFilePath());
    wallet.unlock(password);
    wallet.saveFile(fileInfo.absoluteFilePath());
    wallet.lock();
    wallet.unlock(password);

    wallets.insert(name, wallet);

    savePasswords();

    return password;
}

void EOSWalletManager::open(const QString &name)
{
    QFileInfo info(QDir(dir), name);
    EOSWallet wallet;
    wallet.setWalletFilePath(info.absoluteFilePath());

    if (!wallet.loadFile("")) {
        return;
    }

    QString nameWithoutExt = name;
    QString strEnd = name.right(EOS_WALLET_FILE_EXT.length());
    if (strEnd.compare(EOS_WALLET_FILE_EXT, Qt::CaseInsensitive) == 0) {
        nameWithoutExt = name.left(name.length() - EOS_WALLET_FILE_EXT.length());
    }

    wallets.insert(nameWithoutExt, wallet);
    checkDefaultWallet(nameWithoutExt);
}

void EOSWalletManager::openAll()
{
    QDir walletDir(dir);
    QStringList filters;
    filters << ("*" + EOS_WALLET_FILE_EXT);
    QStringList files = walletDir.entryList(filters, QDir::Files);
    if (files.size()) {
        for (const auto& f : files) {
            open(f);
        }
    }
}

bool EOSWalletManager::isLocked(const QString &name)
{
    if (wallets.find(name) == wallets.end()) {
        // wallet not found!
        return false;
    }

    return wallets[name].isLocked();
}

void EOSWalletManager::lockAll()
{
    for (auto& item : wallets) {
        if (!item.isLocked()) {
            item.lock();
        }
    }
}

void EOSWalletManager::lock(const QString &name)
{
    if (wallets.find(name) == wallets.end()) {
        // wallet not found, should never be here!
        return;
    }

    EOSWallet& wallet = wallets[name];
    if (!wallet.isLocked()) {
        wallet.lock();
    }
}

void EOSWalletManager::unlock(const QString &name, const QString &password)
{
    if (wallets.find(name) == wallets.end()) {
        // wallet not found, should never be here!
        return;
    }

    EOSWallet& wallet = wallets[name];
    if (wallet.isLocked()) {
        wallet.unlock(password);
    }
}

void EOSWalletManager::importKey(const QString &name, const QString &wif)
{
    if (wallets.find(name) == wallets.end()) {
        return;
    }

    EOSWallet& wallet = wallets[name];
    if (wallet.isLocked()) {
        // wallet is locked, nothing we can do.
        return;
    }

    wallet.importKey(wif);
}

QMap<QString, EOSWallet> EOSWalletManager::listKeys(wallet_state state)
{
    QMap<QString, EOSWallet> result;
    for (QMap<QString, EOSWallet>::const_iterator itr = wallets.cbegin();
         itr != wallets.cend(); ++itr) {
        if (state == ws_all || state == itr.value().isLocked()) {
            result.insert(itr.key(), itr.value());
        }
    }

    return result;
}

QVector<QPair<QString, bool> > EOSWalletManager::listWallets(wallet_state state)
{
    QVector<QPair<QString, bool>> result;
    if (wallets.size() == 0) {
        return result;
    }

    for (QMap<QString, EOSWallet>::const_iterator itr = wallets.cbegin();
         itr != wallets.cend(); ++itr) {
        if (state == ws_all || state == itr.value().isLocked()) {
            result.push_back(QPair<QString, bool>(itr.key(), itr.value().isLocked()));
        }
    }

    return result;
}

void EOSWalletManager::loadPasswords()
{
    QString passwdFile = dir;
    if (passwdFile.at(passwdFile.length() - 1) != '/' ||
            passwdFile.at(passwdFile.length() - 1) != '\\')
        passwdFile += '/';
    passwdFile += "password";

    QFile file(passwdFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        return;
    }

    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array.at(i).toObject();
        if (obj.isEmpty()) {
            continue;
        }

        QString key = obj.keys().at(0);
        passwords.insert(key, obj.value(key).toString());
    }
}

void EOSWalletManager::addPasswords(const QString &walletName, const QString &passwd)
{
    if (wallets.find(walletName) == wallets.end()) {
        QMessageBox::warning(nullptr, "Error", "No such wallet.");
        return;
    }

    if (passwords.find(walletName) == passwords.end()) {
        passwords.insert(walletName, passwd);
    } else {
        passwords[walletName] = passwd;
    }
}

QString EOSWalletManager::getPassword(const QString &walletName)
{
    QString passwd;
    if (passwords.find(walletName) != passwords.end()) {
        passwd = passwords.value(walletName);
    }

    return passwd;
}

void EOSWalletManager::savePasswords()
{
    QString passwdFile = dir;
    if (passwdFile.at(passwdFile.length() - 1) != '/' ||
            passwdFile.at(passwdFile.length() - 1) != '\\')
        passwdFile += '/';
    passwdFile += "password";

    QFile file(passwdFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QJsonArray array;
    for (QMap<QString, QString>::const_iterator itr = passwords.constBegin();
         itr != passwords.constEnd(); ++itr) {
        QJsonObject obj;
        obj.insert(itr.key(), QJsonValue(itr.value()));
        array.append(QJsonValue(obj));
    }

    QJsonDocument doc(array);
    file.write(doc.toJson());
}

void EOSWalletManager::signTransaction(SignedTransaction &txn, const std::vector<std::string> &pubKeys, const TypeChainId &cid)
{
    if (pubKeys.empty()) {
        return;
    }

    for (const auto& k : pubKeys) {
        bool found = false;

        for (QMap<QString, EOSWallet>::const_iterator itr = wallets.constBegin();
             itr != wallets.constEnd(); ++itr) {
            if (itr.value().isLocked()) {
                continue;
            }

            std::string wif = itr.value().getPrivateKey(QString::fromStdString(k)).toStdString();
            if (wif.empty()) {
                continue;
            }

            std::vector<unsigned char> priKey = eos_key::get_private_key_by_wif(wif);
            txn.sign(priKey, cid);

            found = true;
            break;
        }

        if (found) {
            break;
        }
    }
}

QString EOSWalletManager::genPassword()
{
    eos_key key;
    return EOS_WALLET_PASSWD_PREFIX + QString::fromStdString(key.get_wif_private_key());
}

void EOSWalletManager::checkDefaultWallet(const QString &fileWithoutExt)
{
    if (!defaultExists && fileWithoutExt == EOS_WALLET_DEFAULT_NAME) {
        defaultExists = true;
    }
}
