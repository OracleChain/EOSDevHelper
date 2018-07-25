#include "accountframe.h"
#include "ui_accountframe.h"
#include "createaccountframe.h"
#include "output/outputframe.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>

AccountFrame::AccountFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AccountFrame),
    createAccountFrame(nullptr),
    createOutPutFrame(nullptr)
{
    ui->setupUi(this);

    initUI();
    initHttpClients();
}

AccountFrame::~AccountFrame()
{
    delete ui;
    httpcs.clear();
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

    edit->setText(std::move(QString::fromStdString(doc.toJson(QJsonDocument::Indented).toStdString())));
}

void AccountFrame::initHttpClients()
{
    httpcs[FunctionID::get_account]             = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::get_transaction]         = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::get_controlled_accounts] = std::make_shared<HttpClient>(nullptr);
}

void AccountFrame::initUI()
{
    createAccountFrame  = new CreateAccountFrame(ui->tabCreateAccount);
    auto createVLayout  = new QVBoxLayout;
    createVLayout->addWidget(createAccountFrame);
    createVLayout->addStretch();
    createVLayout->setSpacing(0);

    createOutPutFrame   = new OutputFrame(ui->tabCreateAccount);
    auto createHLayout  = new QHBoxLayout(nullptr);
    createHLayout->addLayout(createVLayout);
    createHLayout->addWidget(createOutPutFrame);
    createHLayout->setStretch(0, 1);
    createHLayout->setStretch(1, 2);
    ui->tabCreateAccount->setLayout(createHLayout);
}

void AccountFrame::on_pushButtonGetAccount_clicked()
{
    QJsonObject obj;
    obj.insert("account_name", ui->lineEditGetAccountName->text());

    QJsonDocument doc(obj);
    formatPrint(doc, ui->textEditGetAccountInput);

    connect(httpcs[FunctionID::get_account].get(), &HttpClient::responseData, [=](const QByteArray& d){
        formatPrint(QJsonDocument::fromJson(d), ui->textEditGetAccountOutput);
    });
    httpcs[FunctionID::get_account]->request(FunctionID::get_account, doc.toJson());
}

void AccountFrame::on_pushButtonGetTransactions_clicked()
{
    QJsonObject obj;
    obj.insert("id", ui->lineEditGetTransactions->text());

    QJsonDocument doc(obj);
    formatPrint(doc, ui->textEditGetTransactionsInput);

    connect(httpcs[FunctionID::get_transaction].get(), &HttpClient::responseData, [=](const QByteArray& d){
        formatPrint(QJsonDocument::fromJson(d), ui->textEditGetTransactionsOutput);
    });
    httpcs[FunctionID::get_transaction]->request(FunctionID::get_transaction, doc.toJson());
}

void AccountFrame::on_pushButtonGetServants_clicked()
{
    QJsonObject obj;
    obj.insert("controlling_account", ui->lineEditGetServants->text());

    QJsonDocument doc(obj);
    formatPrint(doc, ui->textEditGetServantsInput);

    connect(httpcs[FunctionID::get_controlled_accounts].get(), &HttpClient::responseData, [=](const QByteArray& d){
        formatPrint(QJsonDocument::fromJson(d), ui->textEditGetServantsOutput);
    });
    httpcs[FunctionID::get_controlled_accounts]->request(FunctionID::get_controlled_accounts, doc.toJson());
}
