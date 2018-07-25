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
    auto wallets = EOSWalletManager::instance().listKeys(EOSWalletManager::ws_unlocked);
    for (const auto& w : wallets) {
        auto keys = w.listKeys();
        for (auto itr = keys.cbegin();
             itr != keys.cend(); ++itr) {
            QJsonArray arr;
            arr.push_back(itr.key());
            arr.push_back(itr.value());
            array.append(arr);
        }
    }

    if (array.size()) {
        text = QString::fromStdString(QJsonDocument(array).toJson(QJsonDocument::Indented).toStdString());
    }

    ui->textEditKeys->setText(text);
}

void KeysUnlockedDialog::on_pushButton_clicked()
{
    close();
}
