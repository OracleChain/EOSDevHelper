#include "walletitemframe.h"
#include "ui_walletitemframe.h"

#include "importkeydialog.h"
#include "unlockwalletdialog.h"
#include "eoswalletmanager.h"

#include <QMessageBox>

WalletItemFrame::WalletItemFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WalletItemFrame),
    locked(true)
{
    ui->setupUi(this);
}

WalletItemFrame::~WalletItemFrame()
{
    delete ui;
}

void WalletItemFrame::setWalletName(const QString &name)
{
    ui->labelWalletName->setText(name);
}

void WalletItemFrame::setLockState(bool locked)
{
    this->locked = locked;

    ui->pushButtonLock->setText(locked ? "locked" : "unlocked");
}

void WalletItemFrame::on_pushButtonImportKey_clicked()
{
    if (this->locked) {
        QMessageBox::warning(nullptr, "Error", "Wallet must be unlocked before import keys.");
        return;
    }

    ImportKeyDialog dialog;
    dialog.setWalletName(ui->labelWalletName->text());
    dialog.exec();
}

void WalletItemFrame::on_pushButtonLock_clicked()
{
    if (locked) {
        UnlockWalletDialog dialog;
        dialog.setWalletName(ui->labelWalletName->text());
        dialog.exec();
    } else {
        EOSWalletManager::instance().lock(ui->labelWalletName->text());
    }

    // We should check if the wallet's state was changed, and change the button's text.
    locked = EOSWalletManager::instance().isLocked(ui->labelWalletName->text());
    setLockState(locked);
}
