#ifndef EOSWALLETMANAGER_H
#define EOSWALLETMANAGER_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QPair>

#include "eoswallet.h"

#include "codebase/chain/signedtransaction.h"

class EOSWalletManager
{
public:
    enum wallet_state
    {
        ws_all = -1,
        ws_unlocked = 0,
        ws_locked = 1
    };

public:
    static EOSWalletManager& instance();

    QString create(const QString& name);
    void open(const QString& name);
    void openAll();

    bool defaultWalletExists() { return defaultExists; }
    QString defaultWalletName() { return EOS_WALLET_DEFAULT_NAME; }

    bool isLocked(const QString& name);
    void lockAll();
    void lock(const QString& name);
    void unlock(const QString& name, const QString& password);

    void importKey(const QString& name, const QString& wif);

    QMap<QString, EOSWallet> listKeys(wallet_state state);
    QVector<QPair<QString, bool>> listWallets(wallet_state state);

    void addPasswords(const QString& walletName, const QString& passwd);
    QString getPassword(const QString& walletName);
    void savePasswords();

    void signTransaction(SignedTransaction& txn, const std::vector<std::string>& pubKeys, const TypeChainId& cid);

private:
    EOSWalletManager();

    QString genPassword();
    QString passwordsFile();
    void checkDefaultWallet(const QString& fileWithoutExt);
    void loadPasswords();

private:
    const QString EOS_WALLET_PASSWD_PREFIX  = "PW";
    const QString EOS_WALLET_FILE_EXT       = ".wallet";
    const QString EOS_WALLET_DEFAULT_NAME   = "default";  
    
    QMap<QString, EOSWallet>    wallets;
    QMap<QString, QString>      passwords;
    
    QString dir;
    bool    defaultExists;
};

#endif // EOSWALLETMANAGER_H
