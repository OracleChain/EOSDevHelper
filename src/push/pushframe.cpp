#include "pushframe.h"
#include "ui_pushframe.h"
#include "mainwindow.h"
#include "wallet/eoswalletmanager.h"
#include "actioneditor.h"

#include "codebase/chain/chainmanager.h"
#include "codebase/chain/packedtransaction.h"

#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>

extern MainWindow *w;

PushFrame::PushFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PushFrame)
{
    ui->setupUi(this);

    auto accountVadt = new QRegExpValidator(QRegExp(eos_account_regex), this);
    ui->lineEditContractAccount->setValidator(accountVadt);
    ui->lineEditPermission->setValidator(accountVadt);

    initHttpClients();
}

PushFrame::~PushFrame()
{
    delete ui;

    httpcs.clear();
}

QByteArray PushFrame::packAbiJsonToBinParam()
{
    QByteArray param;

    auto args = ui->textEditAction->toPlainText();
    if (args.isEmpty()) {
        // action args should NOT be empty for now.
        QMessageBox::warning(nullptr, "Error", "Empty action args!");
        return param;
    }

    auto ba  = QByteArray::fromStdString(args.toStdString());
    auto doc = QJsonDocument::fromJson(ba);
    if (doc.isNull()) {
        QMessageBox::warning(nullptr, "Error", "Wrong json format!");
        return param;
    }

    QJsonObject obj;
    obj.insert("code", QJsonValue(ui->lineEditContractAccount->text()));
    obj.insert("action", QJsonValue(ui->comboBoxContractAction->currentData().toString())); // we could use currentText(), but currentData() is a more accurate way.

    if (doc.isObject()) {
        obj.insert("args", doc.object());
    } else if (doc.isArray()) {
        obj.insert("args", doc.array());
    } else {
        return param;
    }

    Q_ASSERT(obj.contains("args"));

    param = QJsonDocument(obj).toJson();
    return param;
}

QByteArray PushFrame::packGetRequiredKeysParam()
{
    auto abiBinObj  = QJsonDocument::fromJson(abiJsonToBinData).object();
    auto binargs    = abiBinObj.value("binargs").toString();
    auto code       = ui->lineEditContractAccount->text();
    auto action     = ui->comboBoxContractAction->currentData().toString();
    auto permission = ui->lineEditPermission->text();

    signedTxn = ChainManager::createTransaction(code.toStdString(), action.toStdString(), binargs.toStdString(),
                                                ChainManager::getActivePermission(permission.toStdString()), getInfoData);

    QJsonArray avaibleKeys;
    auto unlockedWallets = EOSWalletManager::instance().listKeys(EOSWalletManager::ws_unlocked);
    for (const auto& w : unlockedWallets) {
        auto keys = w.listKeys();
        auto list = keys.keys();
        for (auto i = 0; i < list.size(); ++i) {
            avaibleKeys.append(QJsonValue(list.at(i)));
        }
    }

    QJsonObject obj;
    obj.insert("available_keys", avaibleKeys);
    obj.insert("transaction", signedTxn.toJson().toObject());
    return QJsonDocument(obj).toJson();
}

QByteArray PushFrame::packPushTransactionParam()
{
    auto array = QJsonDocument::fromJson(getRequiredKeysData).object().value("required_keys").toArray();
    if (!array.size()) {
        return QByteArray();
    }

    std::vector<std::string> keys;
    for (auto i = 0; i < array.size(); ++i) {
        keys.push_back(array.at(i).toString().toStdString());
    }

    auto infoObj = QJsonDocument::fromJson(getInfoData).object();
    if (infoObj.isEmpty()) {
        return QByteArray();
    }

    EOSWalletManager::instance().signTransaction(signedTxn, keys, TypeChainId::fromHex(infoObj.value("chain_id").toString().toStdString()));
    return QJsonDocument(PackedTransaction(signedTxn, "none").toJson().toObject()).toJson();
}

void PushFrame::updateActionList()
{
    if (contractAbi.isEmpty()) {
        return;
    }

    auto obj = QJsonDocument::fromJson(contractAbi).object();
    if (obj.isEmpty()) {
        return;
    }

    auto abi = obj.value("abi");
    if (!abi.isObject()) {
        return;
    }

    auto actions = abi.toObject().value("actions").toArray();
    if (actions.isEmpty()) {
        return;
    }

    ui->comboBoxContractAction->clear();
    for (auto i = 0; i < actions.size(); ++i) {
        auto tmp = actions.at(i).toObject();
        if (tmp.isEmpty()) {
            continue;
        }

        auto name = tmp.value("name").toString();
        auto type = tmp.value("type").toString();
        ui->comboBoxContractAction->addItem(name, type);
    }
}

void PushFrame::initHttpClients()
{
    httpcs[FunctionID::get_info]            = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::abi_json_to_bin]     = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::get_required_keys]   = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::push_transaction]    = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::get_abi]             = std::make_shared<HttpClient>(nullptr);
}

void PushFrame::on_pushButtonImportFile_clicked()
{
    auto filePath = QFileDialog::getOpenFileName(this, "Choose file", "");
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    auto ba = QByteArray::fromStdString(in.readAll().toStdString());
    auto formatedData = QJsonDocument::fromJson(ba).toJson(QJsonDocument::Indented);
    ui->textEditAction->setText(QString::fromStdString(formatedData.toStdString()));
}

void PushFrame::on_pushButtonSend_clicked()
{
    auto param = packAbiJsonToBinParam();
    if (param.isEmpty()) {
        return;
    }

    w->pushOutputFrame()->clearOutput();
    w->pushOutputFrame()->setRequestOutput(0, "abi_json_to_bin", param);

    connect(httpcs[FunctionID::abi_json_to_bin].get(), &HttpClient::responseData, this, &PushFrame::abi_json_to_bin_returned);
    httpcs[FunctionID::abi_json_to_bin]->request(FunctionID::abi_json_to_bin, param);
}

void PushFrame::abi_json_to_bin_returned(const QByteArray &data)
{
    disconnect(httpcs[FunctionID::abi_json_to_bin].get(), &HttpClient::responseData, this, &PushFrame::abi_json_to_bin_returned);

    w->pushOutputFrame()->setResponseOutput(0, data);

    abiJsonToBinData.clear();
    abiJsonToBinData = data;

    w->pushOutputFrame()->setRequestOutput(1, "get_info", QByteArray());

    connect(httpcs[FunctionID::get_info].get(), &HttpClient::responseData, this, &PushFrame::get_info_returned);
    httpcs[FunctionID::get_info]->request(FunctionID::get_info);
}

void PushFrame::get_info_returned(const QByteArray &data)
{
    disconnect(httpcs[FunctionID::get_info].get(), &HttpClient::responseData, this, &PushFrame::get_info_returned);

    w->pushOutputFrame()->setResponseOutput(1, data);

    getInfoData.clear();
    getInfoData = data;

    auto param = packGetRequiredKeysParam();
    if (param.isEmpty()) {
        return;
    }

    w->pushOutputFrame()->setRequestOutput(2, "get_required_keys", param);

    connect(httpcs[FunctionID::get_required_keys].get(), &HttpClient::responseData, this, &PushFrame::get_required_keys_returned);
    httpcs[FunctionID::get_required_keys]->request(FunctionID::get_required_keys, param);
}

void PushFrame::get_required_keys_returned(const QByteArray &data)
{
    disconnect(httpcs[FunctionID::get_required_keys].get(), &HttpClient::responseData, this, &PushFrame::get_required_keys_returned);

    w->pushOutputFrame()->setResponseOutput(2, data);

    getRequiredKeysData.clear();
    getRequiredKeysData = data;

    auto param = packPushTransactionParam();
    if (param.isEmpty()) {
        return;
    }

    w->pushOutputFrame()->setRequestOutput(3, "push_transaction", param);

    connect(httpcs[FunctionID::push_transaction].get(), &HttpClient::responseData, [=](const QByteArray& d){
        w->pushOutputFrame()->setResponseOutput(3, d);
    });
    httpcs[FunctionID::push_transaction]->request(FunctionID::push_transaction, param);
}

void PushFrame::on_pushButtonGetAbi_clicked()
{
    contractAbi.clear();
    ui->comboBoxContractAction->clear();

    auto contract = ui->lineEditContractAccount->text();
    if (contract.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Empty contract account!");
        return;
    }

    QJsonObject obj;
    obj.insert("account_name", QJsonValue(contract));
    auto param = QJsonDocument(obj).toJson();

    w->pushOutputFrame()->clearOutput();
    w->pushOutputFrame()->setRequestOutput(0, "get_abi", param);

    connect(httpcs[FunctionID::get_abi].get(), &HttpClient::responseData, [&](const QByteArray& d){
        w->pushOutputFrame()->setResponseOutput(0, d);

        auto obj = QJsonDocument::fromJson(d).object();
        if (obj.isEmpty()) {
            return;
        }

        if (obj.contains("code") || obj.contains("error")) {
            return;
        }

        contractAbi = d;
        updateActionList();
    });
    httpcs[FunctionID::get_abi]->request(FunctionID::get_abi, param);
}

void PushFrame::on_pushButtonFormInput_clicked()
{
    auto action = ui->comboBoxContractAction->currentData().toString();
    if (action.isEmpty()) {
        return;
    }

    auto obj = QJsonDocument::fromJson(contractAbi).object();
    if (obj.isEmpty()) {
        return;
    }

    auto abi = obj.value("abi").toObject();
    if (abi.isEmpty()) {
        return;
    }

    auto structs = abi.value("structs").toArray();
    if (structs.isEmpty()) {
        return;
    }

    for (auto i = 0; i < structs.size(); ++i) {
        auto tmp = structs.at(i).toObject();
        if (tmp.isEmpty()) {
            continue;
        }

        auto name = tmp.value("name").toString();
        if (name == action) {
            ActionEditor editor(action, QJsonDocument(tmp.value("fields").toArray()).toJson());
            connect(&editor, &ActionEditor::ActionFinish, [&](const QByteArray& ba){
                ui->textEditAction->setText(QString::fromStdString(ba.toStdString()));
            });
            editor.exec();
            break;
        }
    }
}
