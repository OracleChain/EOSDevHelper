#include "pushframe.h"
#include "ui_pushframe.h"
#include "mainwindow.h"
#include "wallet/eoswalletmanager.h"
#include "actioneditor.h"

#include "codebase/utility/httpclient.h"
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
    ui(new Ui::PushFrame),
    httpc(new HttpClient)
{
    ui->setupUi(this);

    QRegExpValidator *accountVadt = new QRegExpValidator(QRegExp(eos_account_regex), this);
    ui->lineEditContractAccount->setValidator(accountVadt);
    ui->lineEditPermission->setValidator(accountVadt);
}

PushFrame::~PushFrame()
{
    delete ui;
    delete httpc;
}

QByteArray PushFrame::packAbiJsonToBinParam()
{
    QByteArray param;

    QString args = ui->textEditAction->toPlainText();
    if (args.isEmpty()) {
        // action args should NOT be empty for now.
        QMessageBox::warning(nullptr, "Error", "Empty action args!");
        return param;
    }

    QByteArray ba = QByteArray::fromStdString(args.toStdString());
    QJsonDocument doc = QJsonDocument::fromJson(ba);
    if (doc.isNull()) {
        QMessageBox::warning(nullptr, "Error", "Wrong json format!");
        return param;
    }

    QString code = ui->lineEditContractAccount->text();
    QString action = ui->comboBoxContractAction->currentData().toString();  // we could use currentText(), but currentData() is a more accurate way.

    QJsonObject obj;
    obj.insert("code", QJsonValue(code));
    obj.insert("action", QJsonValue(action));

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
    QJsonObject abiBinObj = QJsonDocument::fromJson(abiJsonToBinData).object();
    QString binargs = abiBinObj.value("binargs").toString();
    QString code = ui->lineEditContractAccount->text();
    QString action = ui->comboBoxContractAction->currentData().toString();
    QString permission = ui->lineEditPermission->text();

    signedTxn = ChainManager::createTransaction(code.toStdString(), action.toStdString(), binargs.toStdString(),
                                                ChainManager::getActivePermission(permission.toStdString()), getInfoData);

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

QByteArray PushFrame::packPushTransactionParam()
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

void PushFrame::UpdateActionList()
{
    if (contractAbi.isEmpty()) {
        return;
    }

    QJsonObject obj = QJsonDocument::fromJson(contractAbi).object();
    if (obj.isEmpty()) {
        return;
    }

    QJsonValue abi = obj.value("abi");
    if (!abi.isObject()) {
        return;
    }

    QJsonArray actions = abi.toObject().value("actions").toArray();
    if (actions.isEmpty()) {
        return;
    }

    ui->comboBoxContractAction->clear();
    for (int i = 0; i < actions.size(); ++i) {
        QJsonObject tmp = actions.at(i).toObject();
        if (tmp.isEmpty()) {
            continue;
        }

        QString name = tmp.value("name").toString();
        QString type = tmp.value("type").toString();
        ui->comboBoxContractAction->addItem(name, type);
    }
}

void PushFrame::on_pushButtonImportFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choose file", "");
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString data = in.readAll();    // yeah, since we want import file and edit the content, we just read all.

    QByteArray ba = QByteArray::fromStdString(data.toStdString());
    QJsonDocument doc = QJsonDocument::fromJson(ba);
    QByteArray formatedData = doc.toJson(QJsonDocument::Indented);
    ui->textEditAction->setText(QString::fromStdString(formatedData.toStdString()));
}

void PushFrame::on_pushButtonSend_clicked()
{
    QByteArray param = packAbiJsonToBinParam();
    if (param.isEmpty()) {
        return;
    }

    w->pushOutputFrame()->clearOutput();

    w->pushOutputFrame()->setRequestOutput(0, "abi_json_to_bin", param);

    if (httpc) {
        httpc->request(FunctionID::abi_json_to_bin, param);
        connect(httpc, &HttpClient::responseData, this, &PushFrame::abi_json_to_bin_returned);
    }
}

void PushFrame::abi_json_to_bin_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushFrame::abi_json_to_bin_returned);

    w->pushOutputFrame()->setResponseOutput(0, data);

    abiJsonToBinData.clear();
    abiJsonToBinData = data;

    w->pushOutputFrame()->setRequestOutput(1, "get_info", QByteArray());

    if (httpc) {
        httpc->request(FunctionID::get_info);
        connect(httpc, &HttpClient::responseData, this, &PushFrame::get_info_returned);
    }
}

void PushFrame::get_info_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushFrame::get_info_returned);

    w->pushOutputFrame()->setResponseOutput(1, data);

    getInfoData.clear();
    getInfoData = data;

    QByteArray param = packGetRequiredKeysParam();
    if (param.isEmpty()) {
        return;
    }

    w->pushOutputFrame()->setRequestOutput(2, "get_required_keys", param);

    if (httpc) {
        httpc->request(FunctionID::get_required_keys, param);
        connect(httpc, &HttpClient::responseData, this, &PushFrame::get_required_keys_returned);
    }
}

void PushFrame::get_required_keys_returned(const QByteArray &data)
{
    disconnect(httpc, &HttpClient::responseData, this, &PushFrame::get_required_keys_returned);

    w->pushOutputFrame()->setResponseOutput(2, data);

    getRequiredKeysData.clear();
    getRequiredKeysData = data;

    QByteArray param = packPushTransactionParam();
    if (param.isEmpty()) {
        return;
    }

    w->pushOutputFrame()->setRequestOutput(3, "push_transaction", param);

    if (httpc) {
        httpc->request(FunctionID::push_transaction, param);
        connect(httpc, &HttpClient::responseData, [=](const QByteArray& d){
            w->pushOutputFrame()->setResponseOutput(3, d);
        });
    }
}

void PushFrame::on_pushButtonGetAbi_clicked()
{
    contractAbi.clear();
    ui->comboBoxContractAction->clear();

    QString contract = ui->lineEditContractAccount->text();
    if (contract.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Empty contract account!");
        return;
    }

    QJsonObject obj;
    obj.insert("account_name", QJsonValue(contract));
    QByteArray param = QJsonDocument(obj).toJson();

    w->pushOutputFrame()->setRequestOutput(0, "get_abi", param);

    if (httpc) {
        httpc->request(FunctionID::get_abi, param);
        connect(httpc, &HttpClient::responseData, [&](const QByteArray& d){
            w->pushOutputFrame()->setResponseOutput(0, d);

            QJsonObject obj = QJsonDocument::fromJson(d).object();
            if (obj.isEmpty()) {
                return;
            }

            if (obj.contains("code") || obj.contains("error")) {
                return;
            }

            contractAbi = d;
            UpdateActionList();
        });
    }
}

void PushFrame::on_pushButtonFormInput_clicked()
{
    QString action = ui->comboBoxContractAction->currentData().toString();
    if (action.isEmpty()) {
        return;
    }

    QJsonObject obj = QJsonDocument::fromJson(contractAbi).object();
    if (obj.isEmpty()) {
        return;
    }

    QJsonObject abi = obj.value("abi").toObject();
    if (abi.isEmpty()) {
        return;
    }

    QJsonArray structs = abi.value("structs").toArray();
    if (structs.isEmpty()) {
        return;
    }

    for (int i = 0; i < structs.size(); ++i) {
        QJsonObject tmp = structs.at(i).toObject();
        if (tmp.isEmpty()) {
            continue;
        }

        QString name = tmp.value("name").toString();
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
