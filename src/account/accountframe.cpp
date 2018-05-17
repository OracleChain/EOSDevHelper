#include "accountframe.h"
#include "ui_accountframe.h"
#include "createaccountframe.h"
#include "output/outputframe.h"

#include "codebase/utility/httpclient.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>

AccountFrame::AccountFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AccountFrame),
    createAccountFrame(nullptr),
    createOutPutFrame(nullptr),
    httpcGetAccount(new HttpClient),
    httpcGetTransaction(new HttpClient),
    httpcGetServants(new HttpClient)
{
    ui->setupUi(this);

    createAccountFrame = new CreateAccountFrame(ui->tabCreateAccount);
    QVBoxLayout *createVLayout = new QVBoxLayout;
    createVLayout->addWidget(createAccountFrame);
    createVLayout->addStretch();
    createVLayout->setSpacing(0);

    createOutPutFrame = new OutputFrame(ui->tabCreateAccount);
    QHBoxLayout *createHLayout = new QHBoxLayout(ui->tabCreateAccount);
    createHLayout->addLayout(createVLayout);
    createHLayout->addWidget(createOutPutFrame);
    createHLayout->setStretch(0, 1);
    createHLayout->setStretch(1, 2);
}

AccountFrame::~AccountFrame()
{
    delete ui;
    delete httpcGetAccount;
    delete httpcGetTransaction;
    delete httpcGetServants;
}

void AccountFrame::refreshCreateAccountFrame()
{
    if (createAccountFrame) {
        createAccountFrame->initWallets();
    }
}

void AccountFrame::printCreateAccountInfo(int index, bool request, const QByteArray &data, const QString &title)
{
    if (!createOutPutFrame) {
        return;
    }

    if (request) {
        createOutPutFrame->setRequestOutput(index, title, data);
    } else {
        createOutPutFrame->setResponseOutput(index, data);
    }
}

void AccountFrame::clearOutput()
{
    createOutPutFrame->clearOutput();
}

void AccountFrame::formatPrint(const QJsonDocument &doc, QTextEdit *edit)
{
    if (!edit || doc.isNull()) {
        return;
    }

    QByteArray formatData = doc.toJson(QJsonDocument::Indented);
    edit->setText(std::move(QString::fromStdString(formatData.toStdString())));
}

void AccountFrame::on_pushButtonGetAccount_clicked()
{
    if (httpcGetAccount) {
        QJsonObject obj;
        obj.insert("account_name", ui->lineEditGetAccountName->text());

        QJsonDocument doc(obj);
        formatPrint(doc, ui->textEditGetAccountInput);

        httpcGetAccount->get_account(doc.toJson());
        connect(httpcGetAccount, &HttpClient::responseData, [=](const QByteArray& d){
            formatPrint(QJsonDocument::fromJson(d), ui->textEditGetAccountOutput);
        });
    }
}

void AccountFrame::on_pushButtonGetTransactions_clicked()
{
    if (httpcGetServants) {
        QJsonObject obj;
        obj.insert("account_name", ui->lineEditGetTransactions->text());

        QJsonDocument doc(obj);
        formatPrint(doc, ui->textEditGetTransactionsInput);

        httpcGetTransaction->get_transactions(doc.toJson());
        connect(httpcGetTransaction, &HttpClient::responseData, [=](const QByteArray& d){
            formatPrint(QJsonDocument::fromJson(d), ui->textEditGetTransactionsOutput);
        });
    }
}

void AccountFrame::on_pushButtonGetServants_clicked()
{
    if (httpcGetServants) {
        QJsonObject obj;
        obj.insert("controlling_account", ui->lineEditGetServants->text());

        QJsonDocument doc(obj);
        formatPrint(doc, ui->textEditGetServantsInput);

        httpcGetServants->get_controlled_accounts(doc.toJson());
        connect(httpcGetServants, &HttpClient::responseData, [=](const QByteArray& d){
            formatPrint(QJsonDocument::fromJson(d), ui->textEditGetServantsOutput);
        });
    }
}
