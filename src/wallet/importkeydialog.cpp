#include "importkeydialog.h"
#include "ui_importkeydialog.h"
#include "eoswalletmanager.h"

ImportKeyDialog::ImportKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportKeyDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ImportKeyDialog::~ImportKeyDialog()
{
    delete ui;
}

void ImportKeyDialog::setWalletName(const QString &name)
{
    walletName = name;
    ui->labelWalletName->setText(name);
}

void ImportKeyDialog::on_pushButtonOk_clicked()
{
    EOSWalletManager::instance().importKey(walletName, ui->textEditKey->toPlainText());
    close();
}

void ImportKeyDialog::on_pushButtonCancel_clicked()
{
    close();
}
