#include "walletpassworddialog.h"
#include "ui_walletpassworddialog.h"
#include "eoswalletmanager.h"

WalletPasswordDialog::WalletPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WalletPasswordDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

WalletPasswordDialog::~WalletPasswordDialog()
{
    delete ui;
}

void WalletPasswordDialog::setWalletName(const QString &name)
{
    walletName = name;
}

void WalletPasswordDialog::showPassword(const QString &passwd)
{
    password = passwd;
    ui->textEditPasswd->setText(passwd);
}

void WalletPasswordDialog::on_pushButtonOk_clicked()
{
    if (ui->checkBoxSavePasswd->isChecked()) {
        if (walletName.isEmpty() || password.isEmpty()) {
            return;
        }

        EOSWalletManager::instance().addPasswords(walletName, password);
    }

    close();
}
