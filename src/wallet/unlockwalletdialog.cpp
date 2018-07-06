#include "unlockwalletdialog.h"
#include "ui_unlockwalletdialog.h"

#include "eoswalletmanager.h"

UnlockWalletDialog::UnlockWalletDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnlockWalletDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

UnlockWalletDialog::~UnlockWalletDialog()
{
    delete ui;
}

void UnlockWalletDialog::setWalletName(const QString &name)
{
    this->name = name;
    ui->labelWalletName->setText(name);

    auto passwd = EOSWalletManager::instance().getPassword(name);
    if (!passwd.isEmpty()) {
        ui->textEditPasswd->setText(passwd);
    }
}

void UnlockWalletDialog::on_pushButtonOk_clicked()
{
    EOSWalletManager::instance().unlock(name, ui->textEditPasswd->toPlainText());
    close();
}

void UnlockWalletDialog::on_pushButtonCancel_clicked()
{
    close();
}
