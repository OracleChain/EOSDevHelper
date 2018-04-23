#include "keysunlockeddialog.h"
#include "ui_keysunlockeddialog.h"

#include "eoswalletmanager.h"

#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

KeysUnlockedDialog::KeysUnlockedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeysUnlockedDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    showUnlockedKeys();
}

KeysUnlockedDialog::~KeysUnlockedDialog()
{
    delete ui;
}

void KeysUnlockedDialog::showUnlockedKeys()
{
    QString text = "No keys, try create wallet or unlock wallets.";
    QJsonArray array;
    QMap<QString, EOSWallet> wallets = EOSWalletManager::instance().listKeys(EOSWalletManager::ws_unlocked);
    for (const auto& w : wallets) {
        QMap<QString, QString> keys = w.listKeys();
        for (QMap<QString, QString>::const_iterator itr = keys.cbegin();
             itr != keys.cend(); ++itr) {
            QJsonArray arr;
            arr.push_back(itr.key());
            arr.push_back(itr.value());
            array.append(arr);
        }
    }

    if (array.size()) {
        QJsonDocument doc(array);
        QByteArray formatedData = doc.toJson(QJsonDocument::Indented);
        text = QString::fromStdString(formatedData.toStdString());
    }

    ui->textEditKeys->setText(text);
}

void KeysUnlockedDialog::on_pushButton_clicked()
{
    close();
}
