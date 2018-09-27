#include "tableframe.h"
#include "ui_tableframe.h"

#include "codebase/utility/httpclient.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMessageBox>

TableFrame::TableFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TableFrame)
{
    ui->setupUi(this);

    initHttpClients();
}

TableFrame::~TableFrame()
{
    delete ui;
}

void TableFrame::formatPrint(const QByteArray &data, QTextEdit *edit)
{
    if (!edit) {
        return;
    }

    auto doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return;
    }

    edit->setText(QString::fromStdString(doc.toJson(QJsonDocument::Indented).toStdString()));
}

void TableFrame::updateTableList()
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

    auto tables = abi.toObject().value("tables").toArray();
    if (tables.isEmpty()) {
        return;
    }

    ui->comboBoxTableName->clear();
    for (auto i = 0; i < tables.size(); ++i) {
        auto tmp = tables.at(i).toObject();
        if (tmp.isEmpty()) {
            continue;
        }

        auto name = tmp.value("name").toString();
        ui->comboBoxTableName->addItem(name, name);
    }
}

void TableFrame::initHttpClients()
{
    httpcs[FunctionID::get_table]    = std::make_shared<HttpClient>(nullptr);
    httpcs[FunctionID::get_abi]      = std::make_shared<HttpClient>(nullptr);
}

void TableFrame::on_pushButtonGetTable_clicked()
{
    QJsonObject obj;
    obj.insert("json", QJsonValue(true));
    obj.insert("table_key", QJsonValue(QString()));
    obj.insert("lower_bound", QJsonValue(ui->lineEditTableLowerBound->text()));
    obj.insert("upper_bound", QJsonValue(ui->lineEditTableUpperBound->text()));
    bool ok = false;
    int limit = 10;
    limit = ui->lineEditTableLimit->text().toInt(&ok);
    if (!ok) {
        limit = 10;
    }
    obj.insert("limit", QJsonValue(limit));
    obj.insert("code", QJsonValue(ui->lineEditCode->text()));
    obj.insert("scope", QJsonValue(ui->lineEditScope->text()));
    obj.insert("table", QJsonValue(ui->comboBoxTableName->currentData().toString()));
    if (ui->checkBoxNameKeyType->isChecked()) {
        obj.insert("key_type", QJsonValue("name"));
    }

    QJsonDocument   doc(obj);
    auto            data = doc.toJson();
    formatPrint(data, ui->textEditInput);

    connect(httpcs[FunctionID::get_table].get(), &HttpClient::responseData, [&](const QByteArray& d){
        formatPrint(d, ui->textEditOutput);
    });
    httpcs[FunctionID::get_table]->request(FunctionID::get_table, data);
}

void TableFrame::on_pushButtonGetStruct_clicked()
{
    contractAbi.clear();
    ui->comboBoxTableName->clear();

    auto contract = ui->lineEditCode->text();
    if (contract.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Empty contract account!");
        return;
    }

    QJsonObject obj;
    obj.insert("account_name", QJsonValue(contract));
    auto param = QJsonDocument(obj).toJson();

    formatPrint(param, ui->textEditInput);

    connect(httpcs[FunctionID::get_abi].get(), &HttpClient::responseData, [&](const QByteArray& d){
        formatPrint(d, ui->textEditOutput);

        auto obj = QJsonDocument::fromJson(d).object();
        if (obj.isEmpty()) {
            return;
        }

        if (obj.contains("code") || obj.contains("error")) {
            return;
        }

        contractAbi = d;
        updateTableList();
    });
    httpcs[FunctionID::get_abi]->request(FunctionID::get_abi, param);
}
