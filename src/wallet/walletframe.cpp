#include "walletframe.h"
#include "ui_walletframe.h"
#include "walletitemframe.h"
#include "keysunlockeddialog.h"
#include "walletpassworddialog.h"
#include "eoswalletmanager.h"

#include <QMessageBox>

WalletFrame::WalletFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WalletFrame)
{
    ui->setupUi(this);

    initListUI();
}

WalletFrame::~WalletFrame()
{
    delete ui;
}

void WalletFrame::addWalletToUIList(const QString &name)
{
    auto item   = new QListWidgetItem;
    auto wallet = new WalletItemFrame(this);
    wallet->setWalletName(name);
    wallet->setLockState(EOSWalletManager::instance().isLocked(name));
    item->setSizeHint(wallet->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, wallet);
}

void WalletFrame::initListUI()
{
    EOSWalletManager::instance().openAll();
    auto wallets = EOSWalletManager::instance().listWallets(EOSWalletManager::ws_all);
    for (const auto& w : wallets) {
        addWalletToUIList(w.first);
    }
}

void WalletFrame::on_pushButtonCreateAccount_clicked()
{
    auto wallet_name = ui->lineEditWalletName->text();
    if (wallet_name.isEmpty()) {
        if (EOSWalletManager::instance().defaultWalletExists()) {
            QMessageBox::warning(nullptr, "Error", "default wallet already exists!");
            return;
        }
        wallet_name = EOSWalletManager::instance().defaultWalletName();
    }

    auto passwd = EOSWalletManager::instance().create(wallet_name);
    if (!passwd.isEmpty()) {
        WalletPasswordDialog dialog;
        dialog.setWalletName(wallet_name);
        dialog.showPassword(passwd);
        dialog.exec();

        addWalletToUIList(wallet_name);
    }
}

void WalletFrame::on_pushButtonViewUnlockedKeys_clicked()
{
    KeysUnlockedDialog dialog;
    dialog.exec();
}
