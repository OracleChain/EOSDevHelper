#include "createaccountframe.h"
#include "ui_createaccountframe.h"
#include "wallet/eoswalletmanager.h"
#include "mainwindow.h"

#include "codebase/chain/eosnewaccount.h"
#include "codebase/chain/eosbytewriter.h"
#include "codebase/chain/action.h"
#include "codebase/chain/chainmanager.h"
#include "codebase/ec/typechainid.h"
#include "codebase/chain/packedtransaction.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>

extern MainWindow *w;

CreateAccountFrame::CreateAccountFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CreateAccountFrame)
{
    ui->setupUi(this);
    QRegExpValidator *accountVadt = new QRegExpValidator(QRegExp(eos_account_regex), this);
    ui->lineEditCreatorName->setValidator(accountVadt);
    ui->lineEditNewName->setValidator(accountVadt);

    initHttpClients();
}

CreateAccountFrame::~CreateAccountFrame()
{
    delete ui;

    for (auto itr = httpcs.begin(); itr != httpcs.end(); ++itr) {
        delete itr.value();
    }
    httpcs.clear();
}

void CreateAccountFrame::on_pushButtonOk_clicked()
{
    QString creator = ui->lineEditCreatorName->text();
    QString newAccount = ui->lineEditNewName->text();
    QString wallet = ui->comboBoxWallet->currentData().toString();
    if (creator.isEmpty() || newAccount.isEmpty() || wallet.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Wrong parameter!");
        return;
    }

    w->accountFrame()->clearOutput();

    geneate_keys();

    w->accountFrame()->printCreateAccountInfo(0, true, QByteArray(), "get_info");

    httpcs[FunctionID::get_info]->request(FunctionID::get_info);
    connect(httpcs[FunctionID::get_info], &HttpClient::responseData, this, &CreateAccountFrame::get_info_returned);
}

void CreateAccountFrame::get_info_returned(const QByteArray &data)
{
    disconnect(httpcs[FunctionID::get_info], &HttpClient::responseData, this, &CreateAccountFrame::get_info_returned);

    w->accountFrame()->printCreateAccountInfo(0, false, data, "get_info");

    getInfoData.clear();
    getInfoData = data;

    QByteArray param = packGetRequiredKeysParam();
    w->accountFrame()->printCreateAccountInfo(1, true, param, "get_required_keys");
    if (param.isNull()) {
        return;
    }

    httpcs[FunctionID::get_required_keys]->request(FunctionID::get_required_keys, param);
    connect(httpcs[FunctionID::get_required_keys], &HttpClient::responseData, this, &CreateAccountFrame::get_required_keys_returned);
}

void CreateAccountFrame::get_required_keys_returned(const QByteArray &data)
{
    disconnect(httpcs[FunctionID::get_required_keys], &HttpClient::responseData, this, &CreateAccountFrame::get_required_keys_returned);

    w->accountFrame()->printCreateAccountInfo(1, false, data);

    getRequiredKeysData.clear();
    getRequiredKeysData = data;

    QByteArray param = packPushTransactionParam();
    w->accountFrame()->printCreateAccountInfo(2, true, param, "push_tranaction");
    if (param.isNull()) {
        return;
    }

    httpcs[FunctionID::push_transaction]->request(FunctionID::push_transaction, param);
    connect(httpcs[FunctionID::push_transaction], &HttpClient::responseData, this, &CreateAccountFrame::push_transaction_returned);
}

void CreateAccountFrame::push_transaction_returned(const QByteArray &data)
{
    disconnect(httpcs[FunctionID::push_transaction], &HttpClient::responseData, this, &CreateAccountFrame::push_transaction_returned);

    w->accountFrame()->printCreateAccountInfo(2, false, data);

    QJsonObject obj = QJsonDocument::fromJson(data).object();
    if (obj.isEmpty()) {
        return;
    }

    if (obj.contains("code") || obj.contains("error")) {
        return;
    }

    QString walletName = ui->comboBoxWallet->currentData().toString();
    EOSWalletManager::instance().importKey(walletName, std::move(QString::fromStdString(keys.at(0).get_wif_private_key())));
    EOSWalletManager::instance().importKey(walletName, std::move(QString::fromStdString(keys.at(1).get_wif_private_key())));
}

void CreateAccountFrame::geneate_keys()
{
    keys.clear();

    for (int i = 0; i < 2; ++i) {
        eos_key key;
        keys.push_back(key);

        QString pub = QString::fromStdString(key.get_eos_public_key());

        if (i == 0) {
            ui->textEditOwnerKey->setText(pub);
        } else {
            ui->textEditActiveKey->setText(pub);
        }
    }
}

QByteArray CreateAccountFrame::packGetRequiredKeysParam()
{
    QString creator = ui->lineEditCreatorName->text();
    QString newName = ui->lineEditNewName->text();
    EOSNewAccount newAccount(creator.toStdString(), newName.toStdString(),
                             keys.at(0).get_eos_public_key(), keys.at(1).get_eos_public_key(),
                             creator.toStdString());

    std::vector<unsigned char> hexData = newAccount.dataAsHex();

    signedTxn = ChainManager::createTransaction(EOS_SYSTEM_ACCOUNT, newAccount.getActionName(), std::string(hexData.begin(), hexData.end()),
                                                ChainManager::getActivePermission(creator.toStdString()), getInfoData);
    QJsonObject txnObj = signedTxn.toJson().toObject();

    QJsonArray avaibleKeys;

    QMap<QString, EOSWallet> UnlockedWallets = EOSWalletManager::instance().listKeys(EOSWalletManager::ws_unlocked);
    for (const auto& w : UnlockedWallets) {
        QMap<QString, QString> keys = w.listKeys();
        QStringList list = keys.keys();
        for (int i = 0; i < list.size(); ++i) {
            avaibleKeys.append(QJsonValue(list.at(i)));
        }
    }

    QJsonObject obj;
    obj.insert("available_keys", avaibleKeys);
    obj.insert("transaction", txnObj);
    return QJsonDocument(obj).toJson();
}

QByteArray CreateAccountFrame::packPushTransactionParam()
{
    QJsonArray array = QJsonDocument::fromJson(getRequiredKeysData).object().value("required_keys").toArray();
    if (!array.size()) {
        return QByteArray();
    }

    std::vector<std::string> keys;
    for (int i = 0; i < array.size(); ++i) {
        std::string key = array.at(i).toString().toStdString();
        keys.push_back(key);
    }

    QJsonObject infoObj = QJsonDocument::fromJson(getInfoData).object();
    if (infoObj.isEmpty()) {
        return QByteArray();
    }

    EOSWalletManager::instance().signTransaction(signedTxn, keys, TypeChainId::fromHex(infoObj.value("chain_id").toString().toStdString()));
    PackedTransaction packedTxn(signedTxn, "none");

    QJsonObject obj = packedTxn.toJson().toObject();

    return QJsonDocument(obj).toJson();
}

void CreateAccountFrame::initWallets()
{
    ui->comboBoxWallet->clear();

    QVector<QPair<QString, bool>> wallets = EOSWalletManager::instance().listWallets(EOSWalletManager::ws_unlocked);
    for (QVector<QPair<QString, bool>>::const_iterator itr = wallets.cbegin();
         itr != wallets.cend(); ++itr) {
        ui->comboBoxWallet->addItem(itr->first, itr->first);
    }
}

void CreateAccountFrame::initHttpClients()
{
    httpcs[FunctionID::get_info] = new HttpClient;
    httpcs[FunctionID::get_required_keys] = new HttpClient;
    httpcs[FunctionID::push_transaction] = new HttpClient;
}
